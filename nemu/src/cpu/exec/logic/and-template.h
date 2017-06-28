#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);
//	printf("and:dest = %x    src =   %x  result = %x\n",op_dest->val,op_src->val,result);
	/* TODO: Update EFLAGS. */
//	panic("please implement me");
	OF = 0;
	CF = 0;
	
	EFLAGS_SF(result);
	EFLAGS_ZF(result);
	EFLAGS_PF(result);

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
