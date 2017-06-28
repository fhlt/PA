#include "cpu/exec/template-start.h"

make_helper(ret) {
	EIP = swaddr_read(reg_l(R_ESP),4);
	reg_l(R_ESP) += 4;
	return 1;
}
	

#include "cpu/exec/template-end.h"
