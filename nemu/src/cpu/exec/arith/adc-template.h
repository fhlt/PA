#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute() {
	int64_t result = (int64_t)op_dest->val + (int64_t)op_src->val + CF ;
	OPERAND_W(op_dest, (DATA_TYPE)result);
	/* update EFLAGS */
	EFLAGS_OF(result);
        EFLAGS_SF(result);
        EFLAGS_ZF(result);
        EFLAGS_AF(result);
        EFLAGS_CF(result);
        EFLAGS_PF(result);

        print_asm_template2();
}

make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
