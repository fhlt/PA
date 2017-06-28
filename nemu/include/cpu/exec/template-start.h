#include "cpu/exec/helper.h"

#if DATA_BYTE == 1

#define SUFFIX b
#define DATA_TYPE uint8_t
#define DATA_TYPE_S int8_t

#elif DATA_BYTE == 2

#define SUFFIX w
#define DATA_TYPE uint16_t
#define DATA_TYPE_S int16_t

#elif DATA_BYTE == 4

#define SUFFIX l
#define DATA_TYPE uint32_t
#define DATA_TYPE_S int32_t

#else

#error unknown DATA_BYTE

#endif

#define REG(index) concat(reg_, SUFFIX) (index)
#define REG_NAME(index) concat(regs, SUFFIX) [index]

#define MEM_R(addr) swaddr_read(addr, DATA_BYTE)
#define MEM_W(addr, data) swaddr_write(addr, DATA_BYTE, data)

#define OPERAND_W(op, src) concat(write_operand_, SUFFIX) (op, src)

#define MSB(n) ((DATA_TYPE)(n) >> ((DATA_BYTE << 3) - 1))
/* set CF (carry falg) */
#define EFLAGS_CF(x) \
	CF = (x >> (DATA_BYTE * 8 - 1)) & 0x1
/* set PF (parity flag) */
/* bitNor is also ok */
#define EFLAGS_PF(x)\
	uint8_t tmp = 0x01;\
	uint32_t count = 0;\
	uint32_t i = 0;\
	for(i = 0;i < 8 ; i++)\
	{\
		if(x & tmp) count++;\
		tmp = tmp << 1;\
	}\
	PF = ( count % 2 ) ? 0 : 1
/* set ZF (zero flag) */
#define EFLAGS_ZF(x)\
	ZF = !x
/* ser OF (overflow flag) */
#define EFLAGS_OF(x)\
	OF = ((int64_t)x != (int32_t)x)?1:0
/* set DF (direction flag) */
#define EFLAGS_DF(x)\
	DF = 0
/* set IF (interrupt-enable flag) */
#define EFLAGS_IF(x)\
	IF = 0
/* set SF (sign flag) */
#define EFLAGS_SF(x)\
	SF = (x >= 0)?0:1
/* set AF (assistent Carry Flag) */
#define EFLAGS_AF(x)\
	AF = 0
