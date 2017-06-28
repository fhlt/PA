#include "cpu/exec/template-start.h"

#define instr lea
#if DATA_TYPE == 4
make_helper(lea) {	
	panic("lea");
	return 3;//three bytes instruction
}
#endif



#include "cpu/exec/template-end.h"
