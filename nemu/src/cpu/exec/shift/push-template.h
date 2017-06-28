#include "cpu/exec/template-start.h"

#define instr push

//#if DATA_BYTE == 2 || DATA_BYTE == 4

static void do_execute() {
	reg_l(R_ESP) -= DATA_BYTE;
	MEM_W(reg_l(R_ESP),op_src->val);
	print_asm_template1();
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
