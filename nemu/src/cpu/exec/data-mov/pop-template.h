#include "cpu/exec/template-start.h"

make_helper(concat(pop_r_,SUFFIX)) {
	REG(ops_decoded.opcode & 0x7) = MEM_R(reg_l(R_ESP));	
	reg_l(R_ESP) += DATA_BYTE;
	return 1;//1 byte instruction
}

#include "cpu/exec/template-end.h"
