#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"

typedef int FLOAT;

static inline int F2int(FLOAT a) {
	int val;
	if (a > 0){
		val = a / 0x10000;
	}
	else if(a < 0) {
		a = (~a) + 1;
		a = a / 0x10000;
		val = a | 0x80000000;
	}
	else {
		val = 0;
	}
	return val;
}

static inline FLOAT int2F(int a) {
	FLOAT val;
	if(a > 0) {
		val = a * 0x10000;
	}
	else if(a < 0) {
		a = a & 0xefffffff;
		a = a * 0x10000;
		val = (~a) + 1;
	}
	else {
		val = 0;
	}
	return val;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	nemu_assert(0);
	return 0;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	nemu_assert(0);
	return 0;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif
