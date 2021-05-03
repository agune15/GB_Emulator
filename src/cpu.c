#include <stdlib.h>
#include <stdbool.h>
#include "cpu.h"
#include "registers.h"
#include "instructions.h"
#include "memory.h"

int (*instructions[256])(void) = {
/*0x0*/	NULL, ld_bc_nn, ld_bc_a, NULL, NULL, NULL, ld_b_n, NULL, ld_nnp_sp, NULL, ld_a_bc, NULL, NULL, NULL, ld_c_n, NULL,
/*0x1*/	NULL, ld_de_nn, ld_de_a, NULL, NULL, NULL, ld_d_n, NULL, NULL, NULL, ld_a_de, NULL, NULL, NULL, ld_e_n, NULL,
/*0x2*/	NULL, ld_hl_nn, ldi_hl_a, NULL, NULL, NULL, ld_h_n, NULL, NULL, NULL, ldi_a_hl, NULL, NULL, NULL, ld_l_n, NULL,
/*0x3*/	NULL, ld_sp_nn, ldd_hl_a, NULL, NULL, NULL, ld_hl_n, NULL, NULL, NULL, ldd_a_hl, NULL, NULL, NULL, ld_a_n, NULL,
/*0x4*/	ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_hl, ld_b_a, ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_hl, ld_c_a,
/*0x5*/	ld_d_b, ld_d_c, ld_d_d, ld_d_e, ld_d_h, ld_d_l, ld_d_hl, ld_d_a, ld_e_b, ld_e_c, ld_e_d, ld_e_e, ld_e_h, ld_e_l, ld_e_hl, ld_e_a,
/*0x6*/	ld_h_b, ld_h_c, ld_h_d, ld_h_e, ld_h_h, ld_h_l, ld_h_hl, ld_h_a, ld_l_b, ld_l_c, ld_l_d, ld_l_e, ld_l_h, ld_l_l, ld_l_hl, ld_l_a,
/*0x7*/	ld_hl_b, ld_hl_c, ld_hl_d, ld_hl_e, ld_hl_h, ld_hl_l, NULL, ld_hl_a, ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_hl, ld_a_a,
/*0x8*/	add_a_b, add_a_c, add_a_d, add_a_e, add_a_h, add_a_l, add_a_hl, add_a_a, adc_a_b, adc_a_c, adc_a_d, adc_a_e, adc_a_h, adc_a_l, adc_a_hl, adc_a_a,
/*0x9*/	sub_a_b, sub_a_c, sub_a_d, sub_a_e, sub_a_h, sub_a_l, sub_a_hl, sub_a_a, sbc_a_b, sbc_a_c, sbc_a_d, sbc_a_e, sbc_a_h, sbc_a_l, sbc_a_hl, sbc_a_a,
/*0xA*/	and_a_b, and_a_c, and_a_d, and_a_e, and_a_h, and_a_l, and_a_hl, and_a_a, xor_a_b, xor_a_c, xor_a_d, xor_a_e, xor_a_h, xor_a_l, xor_a_hl, xor_a_a,
/*0xB*/ or_a_b, or_a_c, or_a_d, or_a_e, or_a_h, or_a_l, or_a_hl, or_a_a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xC*/	NULL, pop_bc, NULL, NULL, NULL, push_bc, add_a_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, adc_a_n, NULL,
/*0xD*/	NULL, pop_de, NULL, NULL, NULL, push_de, sub_a_n, NULL, NULL, NULL, NULL, NULL, NULL, NULL, sbc_a_n, NULL,
/*0xE*/	ld_ff_n_a, pop_hl, ld_ff_c_a, NULL, NULL, push_hl, and_a_n, NULL, NULL, NULL, ld_nnp_a, NULL, NULL, NULL, NULL, NULL,
/*0xF*/	ld_a_ff_n, pop_af, ld_a_ff_c, NULL, NULL, push_af, or_a_n, NULL, ld_hl_sp_n, ld_sp_hl, ld_a_nnp, NULL, NULL, NULL, NULL, NULL,
};

registers_t registers;

// Assign the required initial value for each register
void init_registers()
{
	registers.AF = 0x01B0;
	registers.BC = 0x0013;
	registers.DE = 0x00D8;
	registers.HL = 0x014D;
	registers.SP = 0xFFFE;
	registers.PC = 0x100;
}

//TODO: Description
int execute_next_instruction(void)
{
	unsigned char instruction = read_byte(registers.PC++);
	return (*instructions[instruction])();
}

//region Helpers

//region Flags

// Return status of flag
bool is_flag_set(flags_t flag)
{
	return (registers.F >> flag) & 1;
}

// Set a flag
void set_flag(flags_t flag)
{
	registers.F |= (1 << flag);
}

// Clear/reset a flag
void clear_flag(flags_t flag)
{
	registers.F &= ~(1 << flag);
}

//endregion

//region 8-bit loads

// Load byte into register
int load_8bit_vp(unsigned char value, unsigned char *reg, int cycles)
{
	*reg = value;
	return cycles;
}

// Load byte into memory address
int load_8bit_va(unsigned char value, unsigned short address, int cycles)
{
	write_byte(address, value);
	return cycles;
}

// Load byte from address in program memory to register
int load_8bit_vpmp(unsigned char *reg, int cycles)		//TODO: Remove if not used in the future
{
	*reg = read_byte(read_short(registers.PC));
	registers.PC += 2;
	return cycles;
}

//endregion

//region 16-bit loads

// Load short from memory(nn) into register
int load_16bit_nnp(unsigned short *reg, int cycles)
{
	*reg = read_short(registers.PC);
	registers.PC += 2;
	return cycles;
}

//endregion

//region 8-bit adds

// Add byte to register
int add_8bit_vp(unsigned char value, unsigned char *reg, int cycles)
{
	unsigned int addition = *reg + value;

	if (addition > 0xFF)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	int low_nibble_add = (*reg & 0x0F) + (value & 0x0F);
	if (low_nibble_add > 0xF)
		set_flag(HALFCARRY);
	else
		clear_flag(HALFCARRY);

	*reg = (unsigned char)(addition & 0xFF);

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);

	return cycles;
}

// Add byte (+ carry flag) to register
int adc_8bit_vp(unsigned char value, unsigned char *reg, int cycles)
{
	value += (is_flag_set(CARRY)) ? 1 : 0;

	return add_8bit_vp(value, reg, cycles);
}

//endregion

//region 8-bit subs

// Subtract byte from register
int sub_8bit_vp(unsigned char value, unsigned char *reg, int cycles)
{
	set_flag(NEGATIVE);

	if (value > *reg)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	if ((value & 0x0F) > (*reg & 0x0F))
		set_flag(HALFCARRY);
	else
		clear_flag(HALFCARRY);

	*reg -= value;

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	return cycles;
}

// Subtract byte (+ carry flag) from register
int sbc_8bit_vp(unsigned char value, unsigned char *reg, int cycles)
{
	value += (is_flag_set(CARRY)) ? 1 : 0;

	return sub_8bit_vp(value, reg, cycles);
}

//end region

//endregion

//region 8-bit AND

int and_8bit_vp(unsigned char value, unsigned char *reg, int cycles)
{
	*reg &= value;

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	set_flag(HALFCARRY);
	clear_flag(CARRY);

	return cycles;
}

//endregion

//region 8-bit OR

int or_8bit_vp(unsigned char value, unsigned char *reg, int cycles) {
	*reg |= value;

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);
	clear_flag(CARRY);

	return cycles;
}

//endregion

//region 8-bit XOR

int xor_8bit_vp(unsigned char value, unsigned char *reg, int cycles) {
	*reg ^= value;

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);
	clear_flag(CARRY);

	return cycles;
}

//endregion

//region Instructions

// 0x01: Load from memory(nn) to reg-BC
int ld_bc_nn(void) { return load_16bit_nnp(&registers.BC, 12); }

// 0x02: Load from reg-A to memory(BC)
int ld_bc_a(void) { return load_8bit_va(registers.A, registers.BC, 8); }

// 0x06: Load from memory(n) to reg-B
int ld_b_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.B, 8); }

// 0x08: Load from reg-SP to memory address pointed in(nn)
int ld_nnp_sp(void) {
	write_short(read_short(registers.PC), registers.SP);
	registers.PC += 2;
	return 20;
}

// 0x0A: Load from memory(BC) to reg-A
int ld_a_bc(void) { return load_8bit_vp(read_byte(registers.BC), &registers.A, 8); }

// 0x0E: Load from memory(n) to reg-C
int ld_c_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.C, 8); }

// 0x11: Load from memory(nn) to reg-DE
int ld_de_nn(void) { return load_16bit_nnp(&registers.DE, 12); }

// 0x12: Load from reg-A to memory(DE)
int ld_de_a(void) { return load_8bit_va(registers.A, registers.DE, 8); }

// 0x16: Load from memory(n) to reg-D
int ld_d_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.D, 8); }

// 0x1A: Load from memory(DE) to reg-A
int ld_a_de(void) { return load_8bit_vp(read_byte(registers.DE), &registers.A, 8); }

// 0x1E: Load from memory(n) to reg-E
int ld_e_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.E, 8); }

// 0x21: Load from memory(nn) to reg-HL
int ld_hl_nn(void) { return load_16bit_nnp(&registers.HL, 12); }

// 0x22: Load from reg-A to memory(HL), increment reg-HL
int ldi_hl_a(void) { return load_8bit_va(registers.A, registers.HL++, 8); }

// 0x26: Load from memory(n) to reg-H
int ld_h_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.H, 8); }

// 0x2A: Load from memory(HL) to reg-A, increment reg-HL
int ldi_a_hl(void) { return load_8bit_vp(read_byte(registers.HL++), &registers.A, 8); }

// 0x2E: Load from memory(n) to reg-L
int ld_l_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.L, 8); }

// 0x31: Load from memory(nn) to reg-SP
int ld_sp_nn(void) { return load_16bit_nnp(&registers.SP, 12); }

// 0x32: Load from reg-A to memory(HL), decrement reg-HL
int ldd_hl_a(void) { return load_8bit_va(registers.A, registers.HL--, 8); }

// 0x36: Load from memory(n) to memory(HL)
int ld_hl_n(void) { return load_8bit_va(read_byte(registers.PC++), registers.HL, 12); }

// 0x3A: Load from memory(HL) to reg-A, decrement reg-HL
int ldd_a_hl(void) { return load_8bit_vp(read_byte(registers.HL--), &registers.A, 8); }

// 0x3E: Load from memory(n) to reg-A
int ld_a_n(void) { return load_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

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

// 0x47: Load from reg-A to reg-B
int ld_b_a(void) { return load_8bit_vp(registers.A, &registers.B, 4); }

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

// 0x4F: Load from reg-A to reg-C
int ld_c_a(void) { return load_8bit_vp(registers.A, &registers.C, 4); }

// 0x50: Load from reg-B to reg-D
int ld_d_b(void) { return load_8bit_vp(registers.B, &registers.D, 4); }

// 0x51: Load from reg-C to reg-D
int ld_d_c(void) { return load_8bit_vp(registers.C, &registers.D, 4); }

// 0x52: Load from reg-D to reg-D
int ld_d_d(void) { return 4; }

// 0x53: Load from reg-E to reg-D
int ld_d_e(void) { return load_8bit_vp(registers.E, &registers.D, 4); }

// 0x54: Load from reg-H to reg-D
int ld_d_h(void) { return load_8bit_vp(registers.H, &registers.D, 4); }

// 0x55: Load from reg-L to reg-D
int ld_d_l(void) { return load_8bit_vp(registers.L, &registers.D, 4); }

// 0x56: Load from memory(HL) to reg-D
int ld_d_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.D, 8); }

// 0x57: Load from reg-A to reg-D
int ld_d_a(void) { return load_8bit_vp(registers.A, &registers.D, 4); }

// 0x58: Load from reg-B to reg-E
int ld_e_b(void) { return load_8bit_vp(registers.B, &registers.E, 4); }

// 0x59: Load from reg-C to reg-E
int ld_e_c(void) { return load_8bit_vp(registers.C, &registers.E, 4); }

// 0x5A: Load from reg-D to reg-E
int ld_e_d(void) { return load_8bit_vp(registers.D, &registers.E, 4); }

// 0x5B: Load from reg-E to reg-E
int ld_e_e(void) { return 4; }

// 0x5C: Load from reg-H to reg-E
int ld_e_h(void) { return load_8bit_vp(registers.H, &registers.E, 4); }

// 0x5D: Load from reg-L to reg-E
int ld_e_l(void) { return load_8bit_vp(registers.L, &registers.E, 4); }

// 0x5E: Load from memory(HL) to reg-E
int ld_e_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.E, 8); }

// 0x5F: Load from reg-A to reg-E
int ld_e_a(void) { return load_8bit_vp(registers.A, &registers.E, 4); }

// 0x60: Load from reg-B to reg-H
int ld_h_b(void) { return load_8bit_vp(registers.B, &registers.H, 4); }

// 0x61: Load from reg-C to reg-H
int ld_h_c(void) { return load_8bit_vp(registers.C, &registers.H, 4); }

// 0x62: Load from reg-D to reg-H
int ld_h_d(void) { return load_8bit_vp(registers.D, &registers.H, 4); }

// 0x63: Load from reg-E to reg-H
int ld_h_e(void) { return load_8bit_vp(registers.E, &registers.H, 4); }

// 0x64: Load from reg-H to reg-H
int ld_h_h(void) { return 4; }

// 0x65: Load from reg-L to reg-H
int ld_h_l(void) { return load_8bit_vp(registers.L, &registers.H, 4); }

// 0x66: Load from memory(HL) to reg-H
int ld_h_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.H, 8); }

// 0x67: Load from reg-A to reg-H
int ld_h_a(void) { return load_8bit_vp(registers.A, &registers.H, 4); }

// 0x68: Load from reg-B to reg-L
int ld_l_b(void) { return load_8bit_vp(registers.B, &registers.L, 4); }

// 0x69: Load from reg-C to reg-L
int ld_l_c(void) { return load_8bit_vp(registers.C, &registers.L, 4); }

// 0x6A: Load from reg-D to reg-L
int ld_l_d(void) { return load_8bit_vp(registers.D, &registers.L, 4);; }

// 0x6B: Load from reg-E to reg-L
int ld_l_e(void) { return load_8bit_vp(registers.E, &registers.L, 4); }

// 0x6C: Load from reg-H to reg-L
int ld_l_h(void) { return load_8bit_vp(registers.H, &registers.L, 4); }

// 0x6D: Load from reg-L to reg-L
int ld_l_l(void) { return 4; }

// 0x6E: Load from memory(HL) to reg-L
int ld_l_hl(void) { return load_8bit_vp(read_byte(registers.HL), &registers.L, 8); }

// 0x6F: Load from reg-A to reg-L
int ld_l_a(void) { return load_8bit_vp(registers.A, &registers.L, 4); }

// 0x70: Load from reg-B to memory(HL)
int ld_hl_b(void) { return load_8bit_va(registers.B, registers.HL, 8); }

// 0x71: Load from reg-C to memory(HL)
int ld_hl_c(void) { return load_8bit_va(registers.C, registers.HL, 8); }

// 0x72: Load from reg-D to memory(HL)
int ld_hl_d(void) { return load_8bit_va(registers.D, registers.HL, 8); }

// 0x73: Load from reg-E to memory(HL)
int ld_hl_e(void) { return load_8bit_va(registers.E, registers.HL, 8); }

// 0x74: Load from reg-H to memory(HL)
int ld_hl_h(void) { return load_8bit_va(registers.H, registers.HL, 8); }

// 0x75: Load from reg-L to memory(HL)
int ld_hl_l(void) { return load_8bit_va(registers.L, registers.HL, 8); }

// 0x77: Load from reg-A to memory(HL)
int ld_hl_a(void) { return load_8bit_va(registers.A, registers.HL, 8); }

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

// 0x80: Add reg-B to reg-A
int add_a_b(void) { return add_8bit_vp(registers.B, &registers.A, 4); }

// 0x81: Add reg-C to reg-A
int add_a_c(void) { return add_8bit_vp(registers.C, &registers.A, 4); }

// 0x82: Add reg-D to reg-A
int add_a_d(void) { return add_8bit_vp(registers.D, &registers.A, 4); }

// 0x83: Add reg-E to reg-A
int add_a_e(void) { return add_8bit_vp(registers.E, &registers.A, 4); }

// 0x84: Add reg-H to reg-A
int add_a_h(void) { return add_8bit_vp(registers.H, &registers.A, 4); }

// 0x85: Add reg-L to reg-A
int add_a_l(void) { return add_8bit_vp(registers.L, &registers.A, 4); }

// 0x86: Add memory(HL) to reg-A
int add_a_hl(void) { return add_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0x87: Add reg-A to reg-A
int add_a_a(void) { return add_8bit_vp(registers.A, &registers.A, 4); }

// 0x88: Add reg-B (+ carry flag) to reg-A
int adc_a_b(void) { return adc_8bit_vp(registers.B, &registers.A, 4); }

// 0x89: Add reg-C (+ carry flag) to reg-A
int adc_a_c(void) { return adc_8bit_vp(registers.C, &registers.A, 4); }

// 0x8A: Add reg-D (+ carry flag) to reg-A
int adc_a_d(void) { return adc_8bit_vp(registers.D, &registers.A, 4); }

// 0x8B: Add reg-E (+ carry flag) to reg-A
int adc_a_e(void) { return adc_8bit_vp(registers.E, &registers.A, 4); }

// 0x8C: Add reg-H (+ carry flag) to reg-A
int adc_a_h(void) { return adc_8bit_vp(registers.H, &registers.A, 4); }

// 0x8D: Add reg-L (+ carry flag) to reg-A
int adc_a_l(void) { return adc_8bit_vp(registers.L, &registers.A, 4); }

// 0x8E: Add memory(HL) (+ carry flag) to reg-A
int adc_a_hl(void) { return adc_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0x8F: Add reg-A (+ carry flag) to reg-A
int adc_a_a(void) { return adc_8bit_vp(registers.A, &registers.A, 4); }

// 0x90: Subtract reg-B from reg-A
int sub_a_b(void) { return sub_8bit_vp(registers.B, &registers.A, 4); }

// 0x91: Subtract reg-C from reg-A
int sub_a_c(void) { return sub_8bit_vp(registers.C, &registers.A, 4); }

// 0x92: Subtract reg-D from reg-A
int sub_a_d(void) { return sub_8bit_vp(registers.D, &registers.A, 4); }

// 0x93: Subtract reg-E from reg-A
int sub_a_e(void) { return sub_8bit_vp(registers.E, &registers.A, 4); }

// 0x94: Subtract reg-H from reg-A
int sub_a_h(void) { return sub_8bit_vp(registers.H, &registers.A, 4); }

// 0x95: Subtract reg-L from reg-A
int sub_a_l(void) { return sub_8bit_vp(registers.L, &registers.A, 4); }

// 0x96: Subtract memory(HL) from reg-A
int sub_a_hl(void) { return sub_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0x97: Subtract reg-A from reg-A
int sub_a_a(void) { return sub_8bit_vp(registers.A, &registers.A, 4); }

// 0x98: Subtract reg-B (+ carry flag) from reg-A
int sbc_a_b(void) { return sbc_8bit_vp(registers.B, &registers.A, 4); }

// 0x99: Subtract reg-C (+ carry flag) from reg-A
int sbc_a_c(void) { return sbc_8bit_vp(registers.C, &registers.A, 4); }

// 0x9A: Subtract reg-D (+ carry flag) from reg-A
int sbc_a_d(void) { return sbc_8bit_vp(registers.D, &registers.A, 4); }

// 0x9B: Subtract reg-E (+ carry flag) from reg-A
int sbc_a_e(void) { return sbc_8bit_vp(registers.E, &registers.A, 4); }

// 0x9C: Subtract reg-H (+ carry flag) from reg-A
int sbc_a_h(void) { return sbc_8bit_vp(registers.H, &registers.A, 4); }

// 0x9D: Subtract reg-L (+ carry flag) from reg-A
int sbc_a_l(void) { return sbc_8bit_vp(registers.L, &registers.A, 4); }

// 0x9E: Subtract memory(HL) (+ carry flag) from reg-A
int sbc_a_hl(void) { return sbc_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0x9F: Subtract reg-A (+ carry flag) from reg-A
int sbc_a_a(void) { return sbc_8bit_vp(registers.A, &registers.A, 4); }

// 0xA0: Logical AND, reg-B & reg-A, result in reg-A
int and_a_b(void) { return and_8bit_vp(registers.B, &registers.A, 4); }

// 0xA1: Logical AND, reg-C & reg-A, result in reg-A
int and_a_c(void) { return and_8bit_vp(registers.C, &registers.A, 4); }

// 0xA2: Logical AND, reg-D & reg-A, result in reg-A
int and_a_d(void) { return and_8bit_vp(registers.D, &registers.A, 4); }

// 0xA3: Logical AND, reg-E & reg-A, result in reg-A
int and_a_e(void) { return and_8bit_vp(registers.E, &registers.A, 4); }

// 0xA4: Logical AND, reg-H & reg-A, result in reg-A
int and_a_h(void) { return and_8bit_vp(registers.H, &registers.A, 4); }

// 0xA5: Logical AND, reg-L & reg-A, result in reg-A
int and_a_l(void) { return and_8bit_vp(registers.L, &registers.A, 4); }

// 0xA6: Logical AND, memory(HL) & reg-A, result in reg-A
int and_a_hl(void) { return and_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0xA7: Logical AND, reg-A & reg-A, result in reg-A
int and_a_a(void) { return and_8bit_vp(registers.A, &registers.A, 4); }

// 0xA8: Logical XOR, reg-B ^ reg-A, result in reg-A
int xor_a_b(void) { return xor_8bit_vp(registers.B, &registers.A, 4); }

// 0xA9: Logical XOR, reg-C ^ reg-A, result in reg-A
int xor_a_c(void) { return xor_8bit_vp(registers.C, &registers.A, 4); }

// 0xAA: Logical XOR, reg-D ^ reg-A, result in reg-A
int xor_a_d(void) { return xor_8bit_vp(registers.D, &registers.A, 4); }

// 0xAB: Logical XOR, reg-E ^ reg-A, result in reg-A
int xor_a_e(void) { return xor_8bit_vp(registers.E, &registers.A, 4); }

// 0xAC: Logical XOR, reg-H ^ reg-A, result in reg-A
int xor_a_h(void) { return xor_8bit_vp(registers.H, &registers.A, 4); }

// 0xAD: Logical XOR, reg-L ^ reg-A, result in reg-A
int xor_a_l(void) { return xor_8bit_vp(registers.L, &registers.A, 4); }

// 0xAE: Logical XOR, memory(HL) ^ reg-A, result in reg-A
int xor_a_hl(void) { return xor_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0xAF: Logical XOR, reg-A ^ reg-A, result in reg-A
int xor_a_a(void) { return xor_8bit_vp(registers.A, &registers.A, 4); }

// 0xB0: Logical OR, reg-B | reg-A, result in reg-A
int or_a_b(void) { return or_8bit_vp(registers.B, &registers.A, 4); }

// 0xB1: Logical OR, reg-C | reg-A, result in reg-A
int or_a_c(void) { return or_8bit_vp(registers.C, &registers.A, 4); }

// 0xB2: Logical OR, reg-D | reg-A, result in reg-A
int or_a_d(void) { return or_8bit_vp(registers.D, &registers.A, 4); }

// 0xB3: Logical OR, reg-E | reg-A, result in reg-A
int or_a_e(void) { return or_8bit_vp(registers.E, &registers.A, 4); }

// 0xB4: Logical OR, reg-H | reg-A, result in reg-A
int or_a_h(void) { return or_8bit_vp(registers.H, &registers.A, 4); }

// 0xB5: Logical OR, reg-L | reg-A, result in reg-A
int or_a_l(void) { return or_8bit_vp(registers.L, &registers.A, 4); }

// 0xB6: Logical OR, memory(HL) | reg-A, result in reg-A
int or_a_hl(void) { return or_8bit_vp(read_byte(registers.HL), &registers.A, 8); }

// 0xB7: Logical OR, reg-A | reg-A, result in reg-A
int or_a_a(void) { return or_8bit_vp(registers.A, &registers.A, 4); }

// 0xC1: Pop from stack to reg-BC, increment SP twice
int pop_bc(void) {
	registers.BC = pop_short_stack();
	return 12;
}

// 0xC5: Push reg-BC to stack, decrement SP twice
int push_bc(void) {
	push_short_stack(registers.BC);
	return 16;
}

// 0xC6: Add memory(n) to reg-A
int add_a_n(void) { return add_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

// 0xCE: Add memory(n) (+ carry flag) to reg-A
int adc_a_n(void) { return adc_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

// 0xD1: Pop from stack to reg-DE, increment SP twice
int pop_de(void) {
	registers.DE = pop_short_stack();
	return 12;
}

// 0xD5: Push reg-DE to stack, decrement SP twice
int push_de(void) {
	push_short_stack(registers.DE);
	return 16;
}

// 0xD6: Subtract memory(n) from reg-A
int sub_a_n(void) { return sub_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

// 0xDE: Subtract memory(n) (+ carry flag) from reg-A
int sbc_a_n(void) { return sbc_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

// 0xE0: Load from reg-A to memory(0xFF00 + n)
int ld_ff_n_a(void) { return load_8bit_va(registers.A, 0xFF00 + read_byte(registers.PC++), 12); }

// 0xE1: Pop from stack to reg-HL, increment SP twice
int pop_hl(void) {
	registers.HL = pop_short_stack();
	return 12;
}

// 0xE2: Load from reg-A to memory(0xFF00 + reg-C)
int ld_ff_c_a(void) { return load_8bit_va(registers.A, 0xFF00 + registers.C, 8); }

// 0xE5: Push reg-HL to stack, decrement SP twice
int push_hl(void) {
	push_short_stack(registers.HL);
	return 16;
}

// 0xE6: Logical AND, memory(n) & reg-A, result in reg-A
int and_a_n(void) { return and_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

// 0xEA: Load from reg-A to memory address pointed in(nn)
int ld_nnp_a(void) {
	write_byte(read_short(registers.PC), registers.A);
	registers.PC += 2;
	return 16;
}

// 0xF0: Load from memory(0xFF00 + n) to reg-A
int ld_a_ff_n(void) {
	return load_8bit_vp(read_byte(0xFF00 + read_byte(registers.PC++)), &registers.A, 12);
}

// 0xF1: Pop from stack to reg-AF, increment SP twice
int pop_af(void) {
	registers.AF = pop_short_stack();
	return 12;
}

// 0xF2: Load from memory(0xFF00 + reg-C) to reg-A
int ld_a_ff_c(void) { return load_8bit_vp(read_byte(0xFF00 + registers.C), &registers.A, 8); }

// 0xF5: Push reg-AF to stack, decrement SP twice
int push_af(void) {
	push_short_stack(registers.AF);
	return 16;
}

// 0xF6: Logical OR, memory(n) | reg-A, result in reg-A
int or_a_n(void) { return or_8bit_vp(read_byte(registers.PC++), &registers.A, 8); }

// 0xF8: Load from reg-SP + (signed)memory(n) to reg-HL
int ld_hl_sp_n(void) {
	signed char operand = (signed char)read_byte(registers.PC++);
	int addition = registers.SP + operand;

	if(addition > 0xFFFF)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	int low_nibble_add = (registers.SP & 0x0F) + (operand & 0x0F);
	if (low_nibble_add > 0x0F)
		set_flag(HALFCARRY);
	else
		clear_flag(HALFCARRY);

	clear_flag(ZERO);
	clear_flag(NEGATIVE);

	registers.HL = (unsigned short)(addition & 0xFFFF);

	return 12;
}

// 0xF9: Load from reg-HL to reg-SP
int ld_sp_hl(void) {
	registers.SP = registers.HL;
	return 8;
}

// 0xFA: Load from memory address pointed in(nn) to reg-A
int ld_a_nnp(void) {
	registers.A = read_byte(read_short(registers.PC));
	registers.PC += 2;
	return 16;
}

//endregion
