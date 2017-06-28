#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	int64_t result = (int64_t)op_dest->val - (int64_t)op_src->val;
	OPERAND_W(op_dest, (DATA_TYPE)result);
	
	/* Update EFLAGS. */
//	uint64_t uresult = (uint64_t)op_dest->val - (uint64_t)op_src->val;
	/* set CF */
	EFLAGS_CF(result);

	/* set PF */
	EFLAGS_PF(result);

	/* set ZF */
	EFLAGS_ZF(result);

	/* set SF */
	EFLAGS_SF(result);

	/* set OF */
	EFLAGS_OF(result);
//        printf("%d,%d\n",(int64_t)op_dest->val,(int64_t)op_src->val);
	/* 打印操作数和目的操作数 */
	print_asm_template2();
}

#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(rm2r)
#include "cpu/exec/template-end.h"
