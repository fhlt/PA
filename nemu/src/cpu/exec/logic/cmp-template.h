#include "cpu/exec/template-start.h"

#define instr cmp
/* 比较源操作数1和源操作数2，并根据结果设置EFLAGS */
static void do_execute() {
	int64_t result = (int64_t)(DATA_TYPE_S)op_dest->val - (int64_t)(DATA_TYPE_S)op_src->val;

	uint64_t uresult = (uint64_t)op_dest->val - (uint64_t)op_src->val;
	
	printf("%x---%x-----%d\n",op_dest->val,op_src->val,(int32_t)result);
	/* set OF */
	EFLAGS_OF(result);
	/* set SF */
	EFLAGS_SF(result);
	/* set ZF */
	EFLAGS_ZF(result);
	/* set AF */
	EFLAGS_AF(result);
	/* set PF */
	EFLAGS_PF(result);
	/* set CF */
	EFLAGS_CF(uresult);
	print_asm_template2();
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(i2rm)
make_instr_helper(rm2r)
make_instr_helper(i2a)
#include "cpu/exec/template-end.h"
