#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include "common.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <elf.h>

extern char *strtab;
extern Elf32_Sym *symtab;
extern int nr_symtab_entry;

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}
//	printf("waiting for input");
	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}
static int cmd_si(char *args)
{
	int step;
	if(args == NULL)
	{
		step = 1;
	}
	else
	{
		step = atoi(args);
		if(step < 0)
		{
			printf("yout step is negative\n");
			return 0;
		}
	}
	cpu_exec(step);
	return 0;
}
static int cmd_q(char *args)
{
	return -1;
}
static int cmd_info(char *args)
{
	int i;
	if(args == NULL)
	{
		printf("your args is NULL\n");
		return 0;
	}
	if(!strcmp(args,"r"))
	{
		for(i = 0;i < 8;i++)
		{
			printf("%s：%-12u%s：%-12u",regsl[i],reg_l(i),regsw[i],reg_w(i));
			if(i < 4)
			{
				printf("%s：%-12u%s：%-12u\n",regsb[i+4],reg_b(i+4),regsb[i],reg_b(i));
			}
			else
			{
				printf("\n");
			}
		}
	}
	else if(!strcmp(args,"w"))
	{
		WP *wp = get_head();
		if(!wp)
		{
			printf("无监视点被使用\n");
			return 0;
		}
		printf("NO\texpr\t\t\t\tvalue\n");
		while(wp)
		{
			printf("%d\t%-s\t\t\t%-u\n",wp->NO,wp->expr,wp->value);
			wp = wp->next;
		}
	}
	else
	{
		printf("your args is illegal\n");
	}
	return 0;
}
static int cmd_p(char *args)
{
	bool stat = true;
	uint32_t value;
	value  = expr(args,&stat);
	if(stat == true)
	{
		printf("%s =  %u\n",args,value);
	}
	else
	{
		printf("the stat is false");//不正常的输入
	}
	return 0;
}
static int cmd_x(char *args)
{
	int i;
	//printf("%s\n",args);
	if(args == NULL)
	{
		printf("your args is NULL\n");
		return 0;
	}
	char *nchar=NULL;
	int n = 1;
	bool stat = true;
	uint32_t value = 0;
	char *temp = args;
	int tempArgsLen =  strlen(args);
	nchar  = strtok(args," ");
	//printf("%s    %s\n",nchar,args);
	if(strlen(nchar) == tempArgsLen)
	{
		n = 1;
	}
	else
	{
	//	printf("hello \n");
		n = atoi(nchar);
		temp = args+strlen(nchar)+1;
	}
	if(n <= 0)
	{
		printf("your args is invalid\n");
		return 0;
	}
	if(strlen(nchar) == tempArgsLen)
		value = expr(args,&stat);
	else
	{
	//	printf("%s",temp);
	        value = expr(temp,&stat);
	}
	if(stat == true)
	{
		for(i = 0;i < n;i++)
		{
			printf("%x\t",*((uint32_t *)hwa_to_va(value+i*4)));
		//	printf("%x\n",swaddr_read(0x100000+i*4,1));
		}
		printf("\n");
	}
	return 0;
}

static int cmd_w(char *args)
{
	WP *wp = NULL;
	bool stat = true;
	uint32_t value=0;
	if(args == NULL)
	{
		printf("表达式为空，拒绝设置监视点\n");
		return 0;
	} 
	else
	{
	//	char *temp = (char *)malloc(sizeof(char)*(strlen(args)+1));
	//	printf("temp = %s",temp);
		value = expr(args,&stat);
	}
	if(stat)
	{
		wp = new_wp();
		if(wp)
		{	
	//		printf("返回的监视点为%d\n",wp->NO);//这句话导致了奇怪的段错误，好奇怪
        //		fprintf(stderr,"%d\n",wp->NO);
			int len = strlen(args)+1;
	//		printf("----%d\n",len);
			wp->expr = (char *)malloc(sizeof(char)*len);
	//		printf("malloc");
			strcpy(wp->expr,args);
	//		printf("%s",wp->expr);
			wp->value = value;
			//将新的监视点加入到已使用链表中去
			add_to_head(wp);
		}
		else
			printf("监视点不足，设置监视点失败\n");
	}
	else
		printf("表达式出现问题\n");
	return 0;
}
static int cmd_d(char *args)
{
	if(args == NULL)
	{
		printf("无参数\n");
		return 0;
	}
	int n = atoi(args);
//	printf("%d-----",n);
	free_wp(n);
	return 0;
}
/* print stack frame */
static int cmd_bt(char *args)
{
	swaddr_t prev_ebp = cpu.ebp;
	swaddr_t ret_addr = cpu.eip;
	uint32_t stack_num = 0;
	int i;
	while(prev_ebp != 0)
	{
		printf("#%-5d0x%x\t\tin    ",stack_num++,prev_ebp);
		for(i = 0;i < nr_symtab_entry ;i++)
		{
			/* 函数中的其中一条指令 */
			if(ELF32_ST_TYPE(symtab[i].st_info) == STT_FUNC 
					&& ret_addr >= symtab[i].st_value && ret_addr < symtab[i].st_value + symtab[i].st_size)
			{
//				printf("%x----",symtab[i].st_value);
//				printf("%x--%d--",ret_addr,symtab[i].st_size);
//				printf("\n%d\n",symtab[i].st_name);
				printf("%s",strtab + symtab[i].st_name);
				break;
			}
		}
		ret_addr = swaddr_read(prev_ebp+4,4);//refresh ret_addr

		/* 5??????? */
		printf("(argc=4,args[4]={0x%x ",swaddr_read(prev_ebp+8,4));
		printf(",0x%x ",swaddr_read(prev_ebp+12,4));
		printf(",0x%x ",swaddr_read(prev_ebp+16,4));
		printf(",0x%x})\n",swaddr_read(prev_ebp+20,4));
		prev_ebp = swaddr_read(prev_ebp,4);
	}	
	return 0;
}
static int cmd_help(char *args);
static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	/*TODO: Add more commands */
	{"si", "Allow the program to pause after a single step of executing the N command",cmd_si},
	{"info","Print register status or watch point info",cmd_info},
	{"p","Expression evaluation",cmd_p},
	{"x","Scan memory",cmd_x},
	{"w","Set watchPoint",cmd_w},
	{"d","Delete the number of n monitoring points",cmd_d},
	{"bt","Print a stack frame chain",cmd_bt},
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);
		//printf("%s",str);
		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif
		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
