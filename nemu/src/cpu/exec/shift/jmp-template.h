#include "cpu/exec/template-start.h"
#include "cpu/decode/decode.h"
#define instr jmp
#if DATA_BYTE == 1
make_helper(jmp_si_b) {
	concat(decode_si_ ,SUFFIX) (eip + 1);
	EIP += (uint32_t)op_src->val;
//	printf("jmp_si_b = %x\n",op_src->val);
#if DATA_BYTE == 2
	EIP &= 0x0000ffff;
#endif
	return DATA_BYTE + 1;
}
#endif
#if DATA_BYTE == 4
make_helper(jmp_si_v) {
	concat(decode_si_ ,SUFFIX) (eip + 1);
	EIP += (uint32_t)op_src->val;
#if DATA_BYTE == 2
	EIP &= 0x0000ffff;
#endif
	return DATA_BYTE + 1;
}
#endif
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_helper(concat(jmp_rm_, SUFFIX)) {
	concat(decode_rm_, SUFFIX)(eip + 1);
	EIP = (uint32_t)op_src->val;
#if DATA_BYTE == 2
	EIP &= 0x0000ffff;
#endif
	return 0;
}
#endif

#include "cpu/exec/template-end.h"
