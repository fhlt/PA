#include "trap.h"

int add3(int a,int b) {
	int c = a+b;
//	set_bp();
	return c;
}
int add2(int a,int b) {
	int c = add3(a,b);
	return c;
}
int add(int a, int b) {
	int c = add2(a,b);
	return c;
}
/*
int test_data[] = {0,1,2};
int ans[] = {0,0x1,0x2};
#define NR_DATA (sizeof(test_data) / sizeof(test_data[0]))
*/
//int test_data[] = {0,1,2};
int test_data[] = {0, 1, 2, 0x7fffffff, 0x80000000, 0x80000001, 0xfffffffe, 0xffffffff};
int ans[] = {0, 0x1, 0x2, 0x7fffffff, 0x80000000, 0x80000001, 0xfffffffe, 0xffffffff, 0x1, 0x2, 0x3, 0x80000000, 0x80000001, 0x80000002, 0xffffffff, 0, 0x2, 0x3, 0x4, 0x80000001, 0x80000002, 0x80000003, 0, 0x1, 0x7fffffff, 0x80000000, 0x80000001, 0xfffffffe, 0xffffffff, 0, 0x7ffffffd, 0x7ffffffe, 0x80000000, 0x80000001, 0x80000002, 0xffffffff, 0, 0x1, 0x7ffffffe, 0x7fffffff, 0x80000001, 0x80000002, 0x80000003, 0, 0x1, 0x2, 0x7fffffff, 0x80000000, 0xfffffffe, 0xffffffff, 0, 0x7ffffffd, 0x7ffffffe, 0x7fffffff, 0xfffffffc, 0xfffffffd, 0xffffffff, 0, 0x1, 0x7ffffffe, 0x7fffffff, 0x80000000, 0xfffffffd, 0xfffffffe};

int lo = 3;

#define NR_DATA (sizeof(test_data) / sizeof(test_data[0]))

int main() {
	int i,j,ans_idx = 0;
	int loop = 0;
	for(i = 0; i < NR_DATA; i ++) {
	//	nemu_assert(test_data[i] == ans[ans_idx++]);
	//	loop++;
	
		for(j = 0; j < NR_DATA; j ++) {
			nemu_assert(add2(test_data[i], test_data[j]) == ans[ans_idx ++]);
//			add3(i,j);
			loop ++;
		}
		lo++;
//		set_bp();
	
	}
//	nemu_assert(loop == NR_DATA);
	nemu_assert(loop == NR_DATA * NR_DATA);

	HIT_GOOD_TRAP;

	return 0;
}

