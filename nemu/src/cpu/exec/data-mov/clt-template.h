#include "cpu/exec/template-start.h"

#define instr clt

/*
 * 把eax的32位整数扩展为64位，高32位用eax的符号位填充保存到edx
 */

make_helper(cltd) {
#if DATA_BYTE == 2
	REG(R_AX) = (int16_t)REG(R_AX);
#endif
#if DATA_BYTE == 4
	REG(EDX) = (int32_t)REG(EAX);
#endif
	return 1;	
}

#include "cpu/exec/template-end.h"
