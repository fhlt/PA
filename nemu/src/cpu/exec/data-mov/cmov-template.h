#include "cpu/exec/template-start.h"
#include "cpu/decode/decode.h"
#define instr cmov

make_helper(concat(cmovs_rm2r_, SUFFIX)) {
	int instr_len = concat(decode_rm2r_, SUFFIX)(eip + 1);
	if(SF == 1) {
		OPERAND_W(op_dest,op_src->val);	
	}
	return instr_len + 1;
}

#include "cpu/exec/template-end.h"
