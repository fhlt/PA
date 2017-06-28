#include "cpu/exec/template-start.h"
#define instr fld
#if DATA_BYTE == 2
make_helper(flds) {
	return 2 + 1;
}
#endif
#include "cpu/exec/template-end.h"
