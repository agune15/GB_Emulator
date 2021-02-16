#include <stdlib.h>
#include "cpu.h"
#include "registers.h"
#include "instructions.h"
#include "memory.h"

int (*instructions[256])(void) = {
/*0x0*/	NULL, NULL, NULL, NULL, NULL, NULL, ld_b_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_c_n, NULL,
/*0x1*/	NULL, NULL, NULL, NULL, NULL, NULL, ld_d_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_e_n, NULL,
/*0x2*/	NULL, NULL, NULL, NULL, NULL, NULL, ld_h_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_l_n, NULL,
/*0x3*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x4*/	ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_hl, NULL, ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_hl, NULL,
/*0x5*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x6*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x7*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_hl, ld_a_a,
/*0x8*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x9*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xA*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xB*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xC*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xD*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xE*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xF*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

struct registers registers;

// Assign the required initial value for each register
void init_registers(void)
{
	registers.AF = 0x01B0;
	registers.BC = 0x0013;
	registers.DE = 0x00D8;
	registers.HL = 0x014D;
	registers.SP = 0xFFFE;
	registers.PC = 0x100;
}

int execute_next_instruction(void)
{
	unsigned char instruction = read_byte(registers.PC++);
	return (*instructions[instruction])();
}

//region Base methods

//region 8-bit loads

// Load value into pointer
int load_8bit_vp(unsigned char value, unsigned char *storage, int cycles)
{
	*storage = value;
	return cycles;
}

// Load value into memory address
int load_8bit_va(unsigned char value, unsigned short address, int cycles)
{
	write_byte(address, value);
	return cycles;
}

//endregion

//endregion

//region Instructions

// 0x06: 8-bit load from memory to reg-B
int ld_b_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.B, 8); }

// 0x0E: 8-bit load from memory to reg-C
int ld_c_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.C, 8); }

// 0x16: 8-bit load from memory to reg-D
int ld_d_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.D, 8); }

// 0x1E: 8-bit load from memory to reg-E
int ld_e_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.E, 8); }

// 0x26: 8-bit load from memory to reg-H
int ld_h_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.H, 8); }

// 0x2E: 8-bit load from memory to reg-L
int ld_l_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.L, 8); }

// 0x40: Load from reg-B to reg-B
int ld_b_b(void) { return 4; }

// 0x41: Load from reg-C to reg-B
int ld_b_c(void) { return load_8bit_vp(registers.C, &registers.B, 4); }

// 0x42: Load from reg-D to reg-B
int ld_b_d(void) { return load_8bit_vp(registers.D, &registers.B, 4); }

// 0x43: Load from reg-E to reg-B
int ld_b_e(void) { return load_8bit_vp(registers.E, &registers.B, 4); }

// 0x44: Load from reg-H to reg-B
int ld_b_h(void) { return load_8bit_vp(registers.H, &registers.B, 4); }

// 0x45: Load from reg-L to reg-B
int ld_b_l(void) { return load_8bit_vp(registers.L, &registers.B, 4); }

// 0x46: Load from memory(HL) to reg-B
int ld_b_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.B, 8); }

// 0x48: Load from reg-B to reg-C
int ld_c_b(void) { return load_8bit_vp(registers.B, &registers.C, 4); }

// 0x49: Load from reg-C to reg-C
int ld_c_c(void) { return 4; }

// 0x4A: Load from reg-D to reg-C
int ld_c_d(void) { return load_8bit_vp(registers.D, &registers.C, 4); }

// 0x4B: Load from reg-E to reg-C
int ld_c_e(void) { return load_8bit_vp(registers.E, &registers.C, 4); }

// 0x4C: Load from reg-H to reg-C
int ld_c_h(void) { return load_8bit_vp(registers.H, &registers.C, 4); }

// 0x4D: Load from reg-B to reg-C
int ld_c_l(void) { return load_8bit_vp(registers.L, &registers.C, 4); }

// 0x4E: Load from memory(HL) to reg-C
int ld_c_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.C, 8); }

// 0x50: Load from reg-B to reg-D
int ld_d_b(void) { return load_8bit_vp(registers.B, &registers.D, 4); }

// 0x51: Load from reg-C to reg-D
int ld_d_c(void) { return load_8bit_vp(registers.C, &registers.D, 4); }

// 0x52: Load from reg-D to reg-D
int ld_d_d(void) { return 4; }

// 0x53: Load from reg-E to reg-D
int ld_d_e(void) { return load_8bit_vp(registers.E, &registers.C, 4); }

/*
// 0x4C: Load from reg-H to reg-C
int ld_c_h(void) { return load_8bit_vp(registers.H, &registers.C, 4); }

// 0x4D: Load from reg-B to reg-C
int ld_c_l(void) { return load_8bit_vp(registers.L, &registers.C, 4); }

// 0x4E: Load from memory(HL) to reg-C
int ld_c_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.C, 8); }
*/

// 0x78: Load from reg-B to reg-A
int ld_a_b(void) { return load_8bit_vp(registers.B, &registers.A, 4); }

// 0x79: Load from reg-C to reg-A
int ld_a_c(void) { return load_8bit_vp(registers.C, &registers.A, 4); }

// 0x7A: Load from reg-D to reg-A
int ld_a_d(void) { return load_8bit_vp(registers.D, &registers.A, 4); }

// 0x7B: Load from reg-E to reg-A
int ld_a_e(void) { return load_8bit_vp(registers.E, &registers.A, 4); }

// 0x7C: Load from reg-H to reg-A
int ld_a_h(void) { return load_8bit_vp(registers.H, &registers.A, 4); }

// 0x7D: Load from reg-L to reg-A
int ld_a_l(void) { return load_8bit_vp(registers.L, &registers.A, 4); }

// 0x7E: Load from memory(HL) to reg-A
int ld_a_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0x7F: Load from reg-A to reg-A
int ld_a_a(void) { return 4; }

//endregion

