#include "cpu/helper.h"
#include "cpu/decode/modrm.h"

#include "all-instr.h"

typedef int (*helper_fun)(swaddr_t);
static make_helper(_2byte_esc);

#define make_group(name, item0, item1, item2, item3, item4, item5, item6, item7) \
	static helper_fun concat(opcode_table_, name) [8] = { \
	/* 0x00 */	item0, item1, item2, item3, \
	/* 0x04 */	item4, item5, item6, item7  \
	}; \
	static make_helper(name) { \
		ModR_M m; \
		m.val = instr_fetch(eip + 1, 1); \
		return concat(opcode_table_, name) [m.opcode](eip); \
	}
	
/* 0x80 */
make_group(group1_b,
	inv, inv, inv, inv, 
	inv, sub_i2rm_b, inv, cmp_i2rm_b)

/* 0x81 */
make_group(group1_v,
	add_i2rm_v, inv, inv, inv, 
	inv, sub_i2rm_v, inv, cmp_i2rm_v)

/* 0x83 */
make_group(group1_sx_v,
	add_si2rm_v, inv, inv, inv, 
	and_si2rm_v, sub_si2rm_v, xor_si2rm_v, cmp_si2rm_v)

/* 0xc0 */
make_group(group2_i_b,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)

/* 0xc1 */
make_group(group2_i_v,
	inv, inv, inv, inv, 
	shl_rm_imm_v, shr_rm_imm_v, inv, sar_rm_imm_v)

/* 0xd0 */
make_group(group2_1_b,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)

/* 0xd1 */
make_group(group2_1_v,
	inv, inv, inv, inv, 
	inv, shr_rm_imm_v, inv, sar_rm_1_v)

/* 0xd2 */
make_group(group2_cl_b,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)

/* 0xd3 */
make_group(group2_cl_v,
	inv, inv, inv, inv, 
	shl_rm_cl_v, shl_rm_cl_v, inv, inv)

/* 0xf6 */
make_group(group3_b,
	inv, inv, not_rm_b, inv, 
	inv, inv, inv, inv)

/* 0xf7 */
make_group(group3_v,
	inv, inv, not_rm_v, inv, 
	inv, imul_rm2a_v, inv, idiv_rm_v)

/* 0xfe */
make_group(group4,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)

/* 0xff */
make_group(group5,
	inc_rm_v, inv, call_rm_v, inv, 
	jmp_rm_v, jmp_si_b, inv, inv)

make_group(group6,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)

make_group(group7,
	inv, inv, inv, inv, 
	inv, inv, inv, inv)


/* TODO: Add more instructions!!! */

helper_fun opcode_table [256] = {
/* 0x00 */	inv, add_r2rm_v, inv, inv,
/* 0x04 */	add_i2a_b, add_i2a_v, inv, inv,
/* 0x08 */	inv, or_r2rm_v, or_rm2r_b, inv,
/* 0x0c */	inv, inv, inv, _2byte_esc,
/* 0x10 */	adc_r2rm_b, adc_r2rm_v, inv, inv,
/* 0x14 */	inv, inv, inv, inv,
/* 0x18 */	inv, inv, inv, inv,
/* 0x1c */	inv, inv, inv, inv,
/* 0x20 */	and_r2rm_b, and_r2rm_v, and_rm2r_b, and_rm2r_v,
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	sub_r2rm_b, sub_r2rm_v, sub_rm2r_b, sub_rm2r_v,
/* 0x2c */	inv, inv, inv, inv,
/* 0x30 */	inv, xor_r2rm_v, inv, inv,
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	cmp_r2rm_b, cmp_r2rm_v, inv, cmp_rm2r_v,
/* 0x3c */	cmp_i2a_b, inv, inv, inv,
/* 0x40 */	inc_r_v, inv, inv, inv,
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	inv, inv, inv, inv,
/* 0x4c */	inv, inv, inv, inv,
/* 0x50 */	push_r_v, inv, inv, push_r_v,
/* 0x54 */	inv, push_r_v, push_r_v, push_r_v,
/* 0x58 */	inv, inv, inv, pop_r_v,
/* 0x5c */	inv, pop_r_v, pop_r_v, pop_r_v,
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, operand_size, inv,
/* 0x68 */	inv, imul_i_rm2r_v, inv, imul_si_rm2r_v,
/* 0x6c */	inv, inv, inv, inv,
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	je_si_b, jne_si_b, jbe_si_b, ja_si_b,
/* 0x78 */	inv, inv, inv, inv,
/* 0x7c */	jl_si_b, jge_si_b, jle_si_b, jg_si_b,
/* 0x80 */	group1_b, group1_v, inv, group1_sx_v, 
/* 0x84 */	test_r2rm_b, test_r2rm_v, inv, inv,
/* 0x88 */	mov_r2rm_b, mov_r2rm_v, mov_rm2r_b, mov_rm2r_v,
/* 0x8c */	inv, lea, inv, inv,
/* 0x90 */	nop, inv, inv, inv,
/* 0x94 */	inv, inv, inv, inv,
/* 0x98 */	inv, cltd, inv, inv,
/* 0x9c */	inv, inv, inv, inv,
/* 0xa0 */	mov_moffs2a_b, mov_moffs2a_v, mov_a2moffs_b, mov_a2moffs_v,
/* 0xa4 */	movsx_v, movsx_v, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, inv,
/* 0xb0 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb4 */	mov_i2r_b, mov_i2r_b, mov_i2r_b, mov_i2r_b,
/* 0xb8 */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xbc */	mov_i2r_v, mov_i2r_v, mov_i2r_v, mov_i2r_v, 
/* 0xc0 */	group2_i_b, group2_i_v, ret, ret,
/* 0xc4 */	inv, inv, mov_i2rm_b, mov_i2rm_v,
/* 0xc8 */	inv, leave, inv, inv,
/* 0xcc */	int3, inv, inv, inv,
/* 0xd0 */	group2_1_b, group2_1_v, group2_cl_b, group2_cl_v,
/* 0xd4 */	inv, inv, nemu_trap, inv,
/* 0xd8 */	inv, flds, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	call_i_v, jmp_si_v, inv, jmp_si_b,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, rep,
/* 0xf4 */	inv, inv, group3_b, group3_v,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, group4, group5
};

helper_fun _2byte_opcode_table [256] = {
/* 0x00 */	group6, group7, inv, inv, 
/* 0x04 */	inv, inv, inv, inv, 
/* 0x08 */	inv, inv, inv, inv, 
/* 0x0c */	inv, inv, inv, inv, 
/* 0x10 */	inv, inv, inv, inv, 
/* 0x14 */	inv, inv, inv, inv, 
/* 0x18 */	inv, inv, inv, inv, 
/* 0x1c */	inv, inv, inv, inv, 
/* 0x20 */	inv, inv, inv, inv, 
/* 0x24 */	inv, inv, inv, inv,
/* 0x28 */	inv, inv, inv, inv, 
/* 0x2c */	inv, inv, inv, inv, 
/* 0x30 */	inv, inv, inv, inv, 
/* 0x34 */	inv, inv, inv, inv,
/* 0x38 */	inv, inv, inv, inv, 
/* 0x3c */	inv, inv, inv, inv, 
/* 0x40 */	inv, inv, inv, inv, 
/* 0x44 */	inv, inv, inv, inv,
/* 0x48 */	cmovs_rm2r_v, inv, inv, inv, 
/* 0x4c */	inv, inv, inv, inv, 
/* 0x50 */	inv, inv, inv, inv, 
/* 0x54 */	inv, inv, inv, inv,
/* 0x58 */	inv, inv, inv, inv, 
/* 0x5c */	inv, inv, inv, inv, 
/* 0x60 */	inv, inv, inv, inv,
/* 0x64 */	inv, inv, inv, inv,
/* 0x68 */	inv, inv, inv, inv, 
/* 0x6c */	inv, inv, inv, inv, 
/* 0x70 */	inv, inv, inv, inv,
/* 0x74 */	inv, inv, inv, inv,
/* 0x78 */	inv, inv, inv, inv, 
/* 0x7c */	inv, inv, inv, inv, 
/* 0x80 */	inv, inv, inv, inv,
/* 0x84 */	inv, inv, jbe_si_v, inv,
/* 0x88 */	inv, inv, inv, inv, 
/* 0x8c */	jl_si_v, inv, jle_si_v, inv, 
/* 0x90 */	inv, inv, inv, inv,
/* 0x94 */	inv, setne_rm_b, inv, inv,
/* 0x98 */	inv, inv, inv, inv, 
/* 0x9c */	inv, inv, inv, inv, 
/* 0xa0 */	inv, inv, inv, inv, 
/* 0xa4 */	inv, inv, inv, inv,
/* 0xa8 */	inv, inv, inv, inv,
/* 0xac */	inv, inv, inv, imul_rm2r_v,
/* 0xb0 */	inv, inv, inv, inv, 
/* 0xb4 */	inv, inv, mov_rm2r_v, inv, 
/* 0xb8 */	inv, inv, inv, inv,
/* 0xbc */	inv, inv, movsx_rm2r_v, inv,
/* 0xc0 */	inv, inv, inv, inv,
/* 0xc4 */	inv, inv, inv, inv,
/* 0xc8 */	inv, inv, inv, inv,
/* 0xcc */	inv, inv, inv, inv,
/* 0xd0 */	inv, inv, inv, inv,
/* 0xd4 */	inv, inv, inv, inv,
/* 0xd8 */	inv, inv, inv, inv,
/* 0xdc */	inv, inv, inv, inv,
/* 0xe0 */	inv, inv, inv, inv,
/* 0xe4 */	inv, inv, inv, inv,
/* 0xe8 */	inv, inv, inv, inv,
/* 0xec */	inv, inv, inv, inv,
/* 0xf0 */	inv, inv, inv, inv,
/* 0xf4 */	inv, inv, inv, inv,
/* 0xf8 */	inv, inv, inv, inv,
/* 0xfc */	inv, inv, inv, inv
};

make_helper(exec) {
	ops_decoded.opcode = instr_fetch(eip, 1);
	printf("%x\n",eip);
	return opcode_table[ ops_decoded.opcode ](eip);
}

static make_helper(_2byte_esc) {
	eip ++;
	uint32_t opcode = instr_fetch(eip, 1);
	ops_decoded.opcode = opcode | 0x100;
	return _2byte_opcode_table[opcode](eip) + 1; 
}
