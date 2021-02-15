#include <stdlib.h>
#include "cpu.h"
#include "registers.h"
#include "instructions.h"
#include "memory.h"

int (*instructions[256])(void) = {
	NULL, NULL, NULL, NULL, NULL, NULL, ld_b_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_c_n, NULL,	//0x0-
	NULL, NULL, NULL, NULL, NULL, NULL, ld_d_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_e_n, NULL,	//0x1-
	NULL, NULL, NULL, NULL, NULL, NULL, ld_h_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_l_n, NULL,	//0x2-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0x3-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0x4-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0x5-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0x6-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, ld_a_b, ld_a_c, NULL, NULL, NULL, NULL, NULL, ld_a_a,	//0x7-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0x8-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0x9-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0xA-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0xB-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0xC-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0xD-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0xE-
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,		//0xF-
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
int load_8bit_vp(unsigned short value, unsigned short *storage, int cycles)
{
	*storage = value;
	return cycles;
}

// Load value into memory address
int load_8bit_va(unsigned short value, unsigned short address, int cycles)
{
	write_byte(address, value);
	return cycles;
}

//endregion

//endregion

//region Instructions

// 0x06: 8-bit load from memory to reg-B
int ld_b_n(void) { return load_8bit_vp(read_byte(registers.PC++), (unsigned short *)&registers.B, 8); }

// 0x0E: 8-bit load from memory to reg-C
int ld_c_n(void) { return load_8bit_vp(read_byte(registers.PC++), (unsigned short *)&registers.C, 8); }

// 0x16: 8-bit load from memory to reg-D
int ld_d_n(void) { return load_8bit_vp(read_byte(registers.PC++), (unsigned short *)&registers.D, 8); }

// 0x1E: 8-bit load from memory to reg-E
int ld_e_n(void) { return load_8bit_vp(read_byte(registers.PC++), (unsigned short *)&registers.E, 8); }

// 0x26: 8-bit load from memory to reg-H
int ld_h_n(void) { return load_8bit_vp(read_byte(registers.PC++), (unsigned short *)&registers.H, 8); }

// 0x2E: 8-bit load from memory to reg-L
int ld_l_n(void) { return load_8bit_vp(read_byte(registers.PC++), (unsigned short *)&registers.L, 8); }

// 0x78: Load from reg-B to reg-A
int ld_a_b(void) { return load_8bit_vp(registers.B, (unsigned short *)&registers.A, 4); }

// 0x79: Load from reg-C to reg-A
int ld_a_c(void) { return load_8bit_vp(registers.C, (unsigned short *)&registers.A, 4); }

// 0x7F: Load from reg-A to reg-A
int ld_a_a(void) { return 4; }

//endregion

