#include "cpu/exec/template-start.h"
#include "cpu/decode/decode.h"

#define instr movsx

static void do_execute() {
#if DATA_BYTE == 2
	OPERAND_W(op_dest,(int16_t)op_src->val);
#endif
#if DATA_BYTE == 4
	OPERAND_W(op_dest,(int32_t)op_src->val);
#endif	
	print_asm_template2();
}

make_instr_helper(rm2r)

make_helper(concat(movsx_, SUFFIX)) {
	MEM_W(MEM_R(REG(R_ESI)),MEM_R(MEM_R(REG(R_EDI))));
/*	if(DF == 0)
	{
		REG(R_ESI) += DATA_BYTE;
		REG(R_EDI) += DATA_BYTE;
	}
	else
	{
		REG(R_ESI) -= DATA_BYTE;
		REG(R_EDI) -= DATA_BYTE;
	}
*/
	return 0;
}

#include "cpu/exec/template-end.h"
