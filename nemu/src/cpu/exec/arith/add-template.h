#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	int64_t result = (int64_t)op_dest->val + (int64_t)op_src->val;
        OPERAND_W(op_dest, (DATA_TYPE)result);
	
//	printf("%x\n",(int32_t)result);
//	printf("%x+%x\n",op_dest->val,op_src->val);
        /* Update EFLAGS */
	EFLAGS_OF(result);
	EFLAGS_SF(result);
	EFLAGS_ZF(result);
	EFLAGS_AF(result);
	EFLAGS_CF(result);
	EFLAGS_PF(result);

	print_asm_template2();
}
make_instr_helper(r2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(i2a)
make_instr_helper(i2rm)

#include "cpu/exec/template-end.h"
