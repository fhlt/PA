#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

#include <ctype.h>
#include <elf.h>

extern char *strtab;//字符串表
extern Elf32_Sym *symtab;//符号表
extern int nr_symtab_entry;//符号表的表项数目

enum {
	NOTYPE = 256, EQ,REG,HEX,NUMBER,NEQ,AND,OR,DEREF,MINUS,
	ID,
	/* TODO: Add more token types */
};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",NOTYPE},                                          //spaces
	{"[+]", '+'},		               			// plus
	{"==", EQ},						// equal
	{"[$][a-z]+",REG},					//register
	{"[(]",'('},						//left parenthesis
	{"[)]",')'},						//right parenthesis
	{"[0][x][0-9a-fA-F]+", HEX},				//hexadecimal
	{"[0-9]+",NUMBER},					//number
	{"[-]",'-'},						//subtraction
	{"[*]",'*'},						//multipication
	{"[*]",DEREF},						//deref
	{"[-]",MINUS},						//minus
	{"[/]",'/'},						//division
	{"[!][=]",NEQ},						//not equal
	{"[&]{2}",AND},						//and
	{"[|]{2}",OR},						//or
	{"[_a-zA-Z][_0-9a-zA-Z]*",ID},				//id
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
	//	printf("%c",e[position]);
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;


				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE:break;
					default: 
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						nr_token++;break;
				}

				break;
			}
			
		}
		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	//	position++;
	}

	return true; 
}
//int count = 0;
//返回优先级最低的符号
int get_dominant_operator(int p,int q)
{
	int op = -1, i = p, n = 0, mod = 0;//通过mod设置优先级
	for(i=p; i <= q; i++)//查询括号是否符合规范
	{
		if(tokens[i].type == '(')
			n++;
		else if(tokens[i].type == ')')
		{
			if((--n) < 0)
			{
				printf("Invalid experpssion\n");
				return -1;
			}
		}
		else if(n == 0 && (tokens[i].type == '+' || tokens[i].type == '-'))
		{
		//	printf("++++++++++++\n");
			op = i;
			mod = 5;
		}
		else if(n == 0 && (tokens[i].type == '*' || tokens[i].type == '/') && mod<= 4)
		{
		//	printf("**************\n");
			op = i;
			mod = 4;
		}
		else if(n == 0 && (tokens[i].type == EQ || tokens[i].type == NEQ) && mod <=3)
		{
			op = i;
			mod = 3;
		}
		else if(n == 0 && (tokens[i].type == OR || tokens[i].type == AND) && mod <= 2)
		{
			op = i;
			mod = 2;
		}	
		else if(n == 0 && (tokens[i].type == OR || tokens[i].type == AND) && mod < 1)
		{
			op = i;
			mod = 1;
		}
	}
//	printf("count = %d   p = %d q = %d and the op is %d  and i = %d\n",count++,p,q,tokens[op].type,i);
	return op;
}

bool check_parentheses(int p,int q)
{
	if(tokens[p].type == '(' && tokens[q].type == ')')
	{
		//printf("check p = %d , q = %d \n",p,q);
		int i = p+1,n = 1;//将p处的'('初始化在此处
		for(;i<=q;i++)
		{
		/*
			if(n<0)
			{
				printf("the exerpssion is invalid\n");
				return false;
			}
		*/
			if(tokens[i].type == '(')	
				n++;
			else if(tokens[i].type == ')')
			{
				n--;
				if(n <= 0)
					break;
			}
		}
	//	printf("n == %d , i = %d\n",n,i);
		if(n != 0)
			return  false;
		else if(n == 0 && i == q)
			return true;
		
	}	
	return false;
}

//16进制数转10进制
uint32_t hex_to_uint(char *hex)
{
//	printf("%s",hex);
	uint32_t value=0;
	int i,m,n;
	m = strlen(hex);
	for(i = 0;i < m;i++)
	{
		if(hex[i] >= 'A' && hex[i] <= 'F')
			n = hex[i]-'A'+10;
		else if(hex[i] >= 'a' && hex[i] <= 'F')
			n = hex[i]-'a'+10;
		else 
			n = hex[i]-'0';
		value = value*16+n;

	}
	return value;
}

double eval(int p,int q)
{
	if(p>q)
	{
		/* Bad expression */
	}
	else if(p == q)
	{
		//printf("p == q %d",p);
		if(tokens[p].type == NUMBER)	//number
		{
			return (uint32_t)atoi(tokens[p].str);
		}
		else if(tokens[p].type == REG)	//register
		{
			//printf("the register is %s",tokens[p].str);
			int i;
		//	uint32_t value = 0;
			for(i = 0;i < 8 ;i++)
			{
				if(!strcmp(tokens[p].str+1,regsl[i]))
					return (uint32_t)reg_l(i);
				else if(!strcmp(tokens[p].str+1,regsw[i]))
					return (uint32_t)reg_w(i);
				else if(!strcmp(tokens[p].str+1,regsb[i]))
					return (uint32_t)reg_b(i);
			}
		//	printf("%u\n",value);
		//	return value;
		}
		else if(tokens[p].type == HEX)
		{
			return hex_to_uint(tokens[p].str + 2);
		}
		else if(tokens[p].type == ID)
		{
			//printf("this token is ID\n");
			//st_name.包含了该符号名在字符串表中的下标
			//st_info.符号类型和绑定信息
			//st_value.符号相对应的值，这个值跟符号有关，可能是一个绝对值，也可能是一个地址等
			//详见elf符号表结构
			int i = 0;
			for(; i < nr_symtab_entry; i++)
			{
				if(ELF32_ST_TYPE(symtab[i].st_info) == STT_OBJECT && !strcmp(tokens[p].str, strtab + symtab[i].st_name))
				{
					printf("value = %x\n",(uint32_t)swaddr_read(symtab[i].st_value,4));//test
					return symtab[i].st_value;
				}
			}
			printf("this symbols not exit\n");
			return 0;
		}
		else 
		{	
			printf("the expression is invalid");
			return 0;
		}
	}
	else if(check_parentheses(p,q) == true)
	{
	//	printf("p = %d,q = %d\n",p,q);
		return eval(p+1,q-1);
	}
	else
	{
		int op;
		//op = the position of dominant operator in the token expression
		op = get_dominant_operator(p,q);
		uint32_t val1 = eval(p,op - 1);
		uint32_t val2 = eval(op + 1,q);
//		printf("type = %d = = = %d\n",op,tokens[op].type);
		switch(tokens[op].type)
		{
			case '+':return (val1 + val2);
			case '-':return (val1 - val2);
			case '*':return (val1 * val2);
			case '/':
				if(val2 !=0 )
					return (val1 / val2);
				else
				{
					printf("the divisor is zero\n");
					return 0;
				}
			case EQ:
				if(val1 == val2)
					return true;
				else 
					return false;
			case NEQ:
				if(val1 != val2)
					return true;
				else 
					return false;
			case AND:
				if(val1 && val2)
					return  true;
				else
					return false;
			case OR:
				if(val1 || val2)
					return true;
				else 
					return false;
			default:assert(0);
		}
	}
	return 0;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	int i;
	for(i = 0;i < nr_token;i++)
	{
	//	printf("%s          %d\n",tokens[i].str,tokens[i].type);
		if (tokens[i].type == '-' && (i == 0 || tokens[i - 1].type == NUMBER))
		{
			tokens[i].type = MINUS;
		}
		else if (tokens[i].type == '*' && (i == 0|| tokens[i - 1].type == REG))
		{
			tokens[i].type = DEREF;
		}	
	}
	/* TODO: Insert codes to evaluate the expression. */
//	panic("please implement me");
	//printf("the nr_token is %d\n",nr_token);
	return eval(0,nr_token-1);
	return 0;
}

