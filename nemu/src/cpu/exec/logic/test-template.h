#include "cpu/exec/template-start.h"

#define instr test
/* 计算源操作数1和源操作数2的逐位逻辑与，并根据结果设置SF、ZF、PF状态标志 */
static void do_execute() {
	DATA_TYPE result = op_dest->val & op_src->val;
//	printf("test  op_dest = %x  op_src = %x\n",op_dest->val,op_src->val);
//	OPERAND_W(op_dest,result);
	
	CF = 0;
	OF = 0;
	/* set SF */
	EFLAGS_SF(result);
	/* set ZF */
	EFLAGS_ZF(result);
	/* set PF */
	EFLAGS_PF(result);
	
	print_asm_template2();
}

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(i2a)

#include "cpu/exec/template-end.h"
