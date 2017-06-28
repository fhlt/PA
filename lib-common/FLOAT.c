#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	FLOAT result = (a * b) / 0x10000;	
	return result;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	FLOAT result = (a / b) * 0x10000;
	return result;
}

FLOAT f2F(float a) {
	int val = (int)(a * 0x10000);
	return int2F(val);
}

FLOAT Fabs(FLOAT a) {	
	return 0;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

