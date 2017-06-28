#include "trap.h"

int pp;

int is_prime(int n) {
	if(n < 2) return 0;
	int i;
	for(i = 2; i < n; i ++) {
		if(n % i == 0) {
			return 0;
		}
	}
	return 1;
}

int gotbaha(int n) {
	int i;
	for(i = 2; i < n; i ++) {
		if(is_prime(i) && is_prime(n - i)) {
			return 1;
		}
	}
	return 0;
}
/*
int main() {
	nemu_assert(gotbaha(8) == 1);
	HIT_GOOD_TRAP;
	return 0;
}
*/
int main() {
	int n;
	for(n = 4; n <= 100; n += 2) {
		nemu_assert(gotbaha(n) == 1);
	}

	nemu_assert(n == 102);

	HIT_GOOD_TRAP;

	return 0;
}

