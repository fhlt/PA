#include "cpu/exec/template-start.h"

#define instr idiv

static void do_execute() {
	int64_t a;
	int32_t b = (DATA_TYPE_S)op_src->val;
#if DATA_BYTE == 1
	a = (int16_t)reg_w(R_EAX);
#else
	a = ((int64_t)REG(R_EDX) << (DATA_BYTE * 8)) | (int64_t)REG(R_EAX);
#endif
//	printf("idiv   a = %d b = %d\n",(int)a,(int)b);
	int quo = (int)a / (int)b;
	int rem = (int)a % (int)b;
	REG(R_EAX) = quo;
	REG(R_EDX) = rem;

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
