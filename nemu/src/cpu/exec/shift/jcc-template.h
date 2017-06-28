#include "cpu/exec/template-start.h"

#if DATA_BYTE == 1
make_helper(je_si_b) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
//	printf("ZF=%x\n",ZF);
	if(ZF == 1) 
	{
		EIP += (uint32_t)val;
	}
	return 2;//2 byte instruction
}

make_helper(jne_si_b) {
        DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
        if(ZF == 0)
        {
                EIP += (uint32_t)val;
        }
        return 2;
}

make_helper(jg_si_b) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
	if(ZF == 0 && SF == OF)
	{
		EIP += (uint32_t)val;
	}
	return 2;
}

make_helper(ja_si_b) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
	if(CF == 0 && ZF == 0)
	{
		EIP += (uint32_t)val;
	}
	return 2;
}
#endif
	
make_helper(concat(jbe_si_, SUFFIX)) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
//	printf("zf = %d,cf = %d\n,val = %x\n,byte = %d\n",ZF,CF,(uint32_t)val,DATA_BYTE);
	if(ZF == 1 || CF == 1)
	{
		EIP += (uint32_t)val;
	}
#if DATA_BYTE == 2
	EIP &=0xffff;
#endif
	return DATA_BYTE + 1;
}

make_helper(concat(jle_si_, SUFFIX)) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
//	printf("zf = %d   sf = %d   of = %d   \n",ZF,SF,OF);
	if(ZF == 1 || SF != OF)
	{
		EIP += val;
	}
	return DATA_BYTE + 1;
}

make_helper(concat(jl_si_, SUFFIX)) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
	if(SF != OF)
	{
		EIP += val;
	}
	return DATA_BYTE + 1;
}

make_helper(concat(jge_si_, SUFFIX)) {
	DATA_TYPE_S val = (DATA_TYPE_S)instr_fetch(eip + 1,DATA_BYTE);
	if(SF == OF)
	{
		EIP += val;
	}
	return DATA_BYTE + 1;
}
#include "cpu/exec/template-end.h"
