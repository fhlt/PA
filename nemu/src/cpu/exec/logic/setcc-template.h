#include "cpu/exec/template-start.h"

#define instr setcc
#if DATA_BYTE == 1
make_helper(setne_rm_b) {
	uint8_t val = instr_fetch(eip + 1, 1); 
	val = (val >> 3) & 0x7;//寄存器
	printf("val = %d,ZF = %d\n",val,ZF);
	if(ZF == 0)
	{
		REG(val) = 1;
	}
	else
	{
		REG(val) = 0;
	}
	return 2;
}
#endif
#include "cpu/exec/template-end.h"
