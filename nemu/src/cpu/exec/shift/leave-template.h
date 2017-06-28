#include "cpu/exec/template-start.h"

#define  instr leave
/* 高级过程退出 */
make_helper(leave) {
	/* 将ESP设置为EBP */
	REG(R_ESP) = REG(R_EBP);
	/* 弹出EBP */
	REG(R_EBP) = MEM_R(REG(R_ESP));
	REG(R_ESP) += DATA_BYTE;
	return 1;	
} 

#include "cpu/exec/template-end.h"
