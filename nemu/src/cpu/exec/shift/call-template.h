#include "cpu/exec/template-start.h"

#define instr call

//#if DTAT_BYTE == 2 || DATA_BYTE == 4

static void do_execute() {
	uint32_t len = DATA_BYTE;
#if DATA_BYTE == 2	
	len += 1;
#endif
	reg_l(R_ESP) -= 4;
	/* push EIP */
	swaddr_write(reg_l(R_ESP), 4 ,EIP + len);
	/* refresh EIP */
	EIP = (uint32_t)(DATA_TYPE)(EIP + op_src->val);
	print_asm_template1();
}

make_instr_helper(i)

make_helper(concat(call_rm_, SUFFIX)) {
	printf("call_rm");
	int len = concat(decode_rm_, SUFFIX)(eip + 1);
#if DATA_BYTE == 2
	len += 1;
#endif
	reg_l(R_ESP) -= 4;
	swaddr_write(reg_l(R_ESP), 4, cpu.eip + len);

	/* refresh eip */
	cpu.eip = op_src->val;
#if DATA_BYTE == 2
	cpu.eip &= 0xffff;
#endif
	return 0;
}

//#endif
#include "cpu/exec/template-end.h"
