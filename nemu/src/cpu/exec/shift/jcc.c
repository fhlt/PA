#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper_v(jl_si)
make_helper_v(jle_si)
make_helper_v(jbe_si)
make_helper_v(jge_si)
