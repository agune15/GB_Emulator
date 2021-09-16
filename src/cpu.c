#include <stdlib.h>
#include "cpu.h"
#include "registers.h"
#include "instructions.h"
#include "memory.h"
#include "cb.h"
#include "interrupts.h"

int (*instructions[256])(void) = {
/*0x0*/	nop, ld_bc_nn, ld_bc_a, inc_bc, inc_b, dec_b, ld_b_n, rlca, ld_nnp_sp, add_hl_bc, ld_a_bc, dec_bc, inc_c, dec_c, ld_c_n, rrca,
/*0x1*/	stop, ld_de_nn, ld_de_a, inc_de, inc_d, dec_d, ld_d_n, rla, jr_n, add_hl_de, ld_a_de, dec_de, inc_e, dec_e, ld_e_n, rra,
/*0x2*/	jr_nz_n, ld_hl_nn, ldi_hl_a, inc_hl, inc_h, dec_h, ld_h_n, daa, jr_z_n, add_hl_hl, ldi_a_hl, dec_hl, inc_l, dec_l, ld_l_n, cpl_a,
/*0x3*/	jr_nc_n, ld_sp_nn, ldd_hl_a, inc_sp, inc_hlp, dec_hlp, ld_hl_n, scf, jr_c_n, add_hl_sp, ldd_a_hl, dec_sp, inc_a, dec_a, ld_a_n, ccf,
/*0x4*/	ld_b_b, ld_b_c, ld_b_d, ld_b_e, ld_b_h, ld_b_l, ld_b_hl, ld_b_a, ld_c_b, ld_c_c, ld_c_d, ld_c_e, ld_c_h, ld_c_l, ld_c_hl, ld_c_a,
/*0x5*/	ld_d_b, ld_d_c, ld_d_d, ld_d_e, ld_d_h, ld_d_l, ld_d_hl, ld_d_a, ld_e_b, ld_e_c, ld_e_d, ld_e_e, ld_e_h, ld_e_l, ld_e_hl, ld_e_a,
/*0x6*/	ld_h_b, ld_h_c, ld_h_d, ld_h_e, ld_h_h, ld_h_l, ld_h_hl, ld_h_a, ld_l_b, ld_l_c, ld_l_d, ld_l_e, ld_l_h, ld_l_l, ld_l_hl, ld_l_a,
/*0x7*/	ld_hl_b, ld_hl_c, ld_hl_d, ld_hl_e, ld_hl_h, ld_hl_l, halt, ld_hl_a, ld_a_b, ld_a_c, ld_a_d, ld_a_e, ld_a_h, ld_a_l, ld_a_hl, ld_a_a,
/*0x8*/	add_a_b, add_a_c, add_a_d, add_a_e, add_a_h, add_a_l, add_a_hl, add_a_a, adc_a_b, adc_a_c, adc_a_d, adc_a_e, adc_a_h, adc_a_l, adc_a_hl, adc_a_a,
/*0x9*/	sub_a_b, sub_a_c, sub_a_d, sub_a_e, sub_a_h, sub_a_l, sub_a_hl, sub_a_a, sbc_a_b, sbc_a_c, sbc_a_d, sbc_a_e, sbc_a_h, sbc_a_l, sbc_a_hl, sbc_a_a,
/*0xA*/	and_a_b, and_a_c, and_a_d, and_a_e, and_a_h, and_a_l, and_a_hl, and_a_a, xor_a_b, xor_a_c, xor_a_d, xor_a_e, xor_a_h, xor_a_l, xor_a_hl, xor_a_a,
/*0xB*/ or_a_b, or_a_c, or_a_d, or_a_e, or_a_h, or_a_l, or_a_hl, or_a_a, cp_a_b, cp_a_c, cp_a_d, cp_a_e, cp_a_h, cp_a_l, cp_a_hl, cp_a_a,
/*0xC*/	ret_nz, pop_bc, jp_nz_nn, jp_nn, call_nz_nn, push_bc, add_a_n, rst_0, ret_z, ret, jp_z_nn, cb, call_z_nn, call_nn, adc_a_n, rst_8,
/*0xD*/	ret_nc, pop_de, jp_nc_nn, NULL, call_nc_nn, push_de, sub_a_n, rst_10, ret_c, reti, jp_c_nn, NULL, call_c_nn, NULL, sbc_a_n, rst_18,
/*0xE*/	ld_ff_n_a, pop_hl, ld_ff_c_a, NULL, NULL, push_hl, and_a_n, rst_20, add_sp_n, jp_hl, ld_nnp_a, NULL, NULL, NULL, xor_a_n, rst_28,
/*0xF*/	ld_a_ff_n, pop_af, ld_a_ff_c, di, NULL, push_af, or_a_n, rst_30, ld_hl_sp_n, ld_sp_hl, ld_a_nnp, ei, NULL, NULL, cp_a_n, rst_38,
};

bool cpu_stopped = false;
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

// Execute next CPU instruction
int execute_next_instruction(void)
{
	if (cpu_stopped)
		return 0;

	unsigned char instruction = read_byte(registers.PC++);
	return (*instructions[instruction])();
}

// Resume the CPU if stopped
void resume_cpu(void)
{
	cpu_stopped = false;
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

// Reset a flag
void reset_flag(flags_t flag)
{
	registers.F &= ~(1 << flag);
}

//endregion

//region Loads

// Load byte into register
int load_8bit_reg(unsigned char value, unsigned char *reg, int cycles)
{
	*reg = value;
	return cycles;
}

// Load byte into memory address
int load_8bit_mem(unsigned char value, unsigned short address, int cycles)
{
	write_byte(address, value);
	return cycles;
}

// Load short from memory(nn) into register
int load_16bit_nn_reg(unsigned short *reg, int cycles)
{
	*reg = read_short(registers.PC);
	registers.PC += 2;
	return cycles;
}

//endregion

//region Adds

// Add byte to register
int add_8bit_reg(unsigned char value, unsigned char *reg, int cycles)
{
	unsigned int result = *reg + value;

	if (result > 0xFF)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	int low_nibble_add = (*reg & 0x0F) + (value & 0x0F);
	if (low_nibble_add > 0x0F)
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	*reg = (unsigned char)(result & 0xFF);

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);

	return cycles;
}

// Add byte (+ C-flag) to register
int adc_8bit_reg(unsigned char value, unsigned char *reg, int cycles)
{
	value += (is_flag_set(CARRY)) ? 1 : 0;

	return add_8bit_reg(value, reg, cycles);
}

// Add short to reg-HL
int add_16bit_hl(unsigned short value, int cycles)
{
	unsigned long result = value + registers.HL;

	if ((registers.HL & 0x0FFF + result & 0x0FFF) > 0x0FFF)
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	registers.HL = (unsigned short)(result & 0xFFFF);

	if (result > 0xFFFF) {
		set_flag(CARRY);
	}
	else {
		reset_flag(CARRY);
	}

	reset_flag(NEGATIVE);

	return cycles;
}

//endregion

//region Subtracts

// Subtract byte from register
int sub_reg(unsigned char value, unsigned char *reg, int cycles)
{
	set_flag(NEGATIVE);

	if (value > *reg)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if ((value & 0x0F) > (*reg & 0x0F))
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	*reg -= value;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	return cycles;
}

// Subtract byte (+ C-flag) from register
int sbc_reg(unsigned char value, unsigned char *reg, int cycles)
{
	value += (is_flag_set(CARRY)) ? 1 : 0;

	return sub_reg(value, reg, cycles);
}

//end region

//endregion

//region AND

// Logical AND between byte and register
int and_reg(unsigned char value, unsigned char *reg, int cycles)
{
	*reg &= value;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	set_flag(HALFCARRY);
	reset_flag(CARRY);

	return cycles;
}

//endregion

//region OR

// Logical OR between byte and register
int or_reg(unsigned char value, unsigned char *reg, int cycles)
{
	*reg |= value;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(CARRY);

	return cycles;
}

//endregion

//region XOR

// Logical XOR between byte and register
int xor_reg(unsigned char value, unsigned char *reg, int cycles)
{
	*reg ^= value;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(CARRY);

	return cycles;
}

//endregion

//region Compare

// Compare byte with register
int cp_reg(unsigned char value, unsigned char *reg, int cycles)
{
	set_flag(NEGATIVE);

	if (value == *reg)
		set_flag(ZERO);
	else
		reset_flag(ZERO);

	if (value > *reg)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if ((value & 0x0F) > (*reg & 0x0F))
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	return cycles;
}

//endregion

//region Increments

// Increment register
int inc_8bit_p(unsigned char *reg, int cycles)
{
	if ((*reg & 0x0F) == 0x0F)
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	(*reg)++;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);

	return cycles;
}

// Increment byte from address
int inc_8bit_a(unsigned short address, int cycles) {
	unsigned char byte = read_byte(address);

	if ((byte & 0x0F) == 0x0F)
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	byte++;
	write_byte(address, byte);

	if (byte)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);

	return cycles;
}

// Increment 16-bit register
int inc_16bit_p(unsigned short *reg, int cycles)
{
	(*reg)++;

	return cycles;
}

//endregion

//region Decrements

// Decrement register
int dec_8bit_reg(unsigned char *reg, int cycles)
{
	if (*reg & 0x0F)
		reset_flag(HALFCARRY);
	else
		set_flag(HALFCARRY);

	(*reg)--;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	set_flag(NEGATIVE);

	return cycles;
}

// Decrement byte from address
int dec_8bit_mem(unsigned short address, int cycles)
{
	unsigned char byte = read_byte(address);

	if (byte & 0x0F)
		reset_flag(HALFCARRY);
	else
		set_flag(HALFCARRY);

	byte--;
	write_byte(address, byte);

	if (byte)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	set_flag(NEGATIVE);

	return cycles;
}

// Decrement 16-bit register
int dec_16bit_reg(unsigned short *reg, int cycles)
{
	(*reg)--;

	return cycles;
}

//endregion

//region Jumps

// Jump to address in memory(nn)
int jump_nn(void)
{
	unsigned short address = read_short(registers.PC);
	registers.PC += 2;
	registers.PC = address;
	return 12;
}

// Jump to current address + memory(n)
int jump_n(void)
{
	signed char add_address = (signed char) read_byte(registers.PC++);
	registers.PC += add_address;
	return 8;
}

//endregion

//region Calls

// Push next instruction address to stack and jump to address in memory(nn) (Implemented in instruction 0xCD)
int call_nn(void);

//endregion

//region Resets

// Push current address to stack and jump to given address
int restart(unsigned short address)
{
	push_short_stack(registers.PC);

	registers.PC = address;

	return 32;
}

//endregion

//region Returns

// Pop two bytes from stack and jump to that address (Implemented in instruction 0xC9)
int ret(void);

//endregion

//endregion

//region Instructions

// 0x00: No operation
int nop(void) { return 4; }

// 0x01: Load from memory(nn) to reg-BC
int ld_bc_nn(void) { return load_16bit_nn_reg(&registers.BC, 12); }

// 0x02: Load from reg-A to memory(BC)
int ld_bc_a(void) { return load_8bit_mem(registers.A, registers.BC, 8); }

// 0x03: Increment reg-BC
int inc_bc(void) { return inc_16bit_p(&registers.BC, 8); }

// 0x04: Increment reg-B
int inc_b(void) { return inc_8bit_p(&registers.B, 4); }

// 0x05: Decrement reg-B
int dec_b(void) { return dec_8bit_reg(&registers.B, 4); }

// 0x06: Load from memory(n) to reg-B
int ld_b_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.B, 8); }

// 0x07: Rotate reg-A left (+ new C-flag), set C-flag with MSB
int rlca(void) {
	unsigned char regA_msb = registers.A >> 7;

	registers.A <<= 1;
	registers.A |= regA_msb;

	if (regA_msb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(ZERO);

	return 4;
}

// 0x08: Load from reg-SP to memory address pointed in memory(nn)
int ld_nnp_sp(void) {
	write_short(read_short(registers.PC), registers.SP);
	registers.PC += 2;
	return 20;
}

// 0x09: Add reg-BC to reg-HL
int add_hl_bc(void) { return add_16bit_hl(registers.BC, 8); }

// 0x0A: Load from memory(BC) to reg-A
int ld_a_bc(void) { return load_8bit_reg(read_byte(registers.BC), &registers.A, 8); }

// 0x0B: Decrement reg-BC
int dec_bc(void) { return dec_16bit_reg(&registers.BC, 8); }

// 0x0C: Increment reg-C
int inc_c(void) { return inc_8bit_p(&registers.C, 4); }

// 0x0D: Decrement reg-C
int dec_c(void) { return dec_8bit_reg(&registers.C, 4); }

// 0x0E: Load from memory(n) to reg-C
int ld_c_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.C, 8); }

// 0x0F: Rotate reg-A right (+ new C-flag), set C-flag with LSB
int rrca(void) {
	unsigned char regA_lsb = registers.A << 7;

	registers.A >>= 1;
	registers.A |= regA_lsb;

	if (regA_lsb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(ZERO);

	return 4;
}

// 0x10: STOP Interrupt
int stop(void) {
	cpu_stopped = true;
	return 4;
}

// 0x11: Load from memory(nn) to reg-DE
int ld_de_nn(void) { return load_16bit_nn_reg(&registers.DE, 12); }

// 0x12: Load from reg-A to memory(DE)
int ld_de_a(void) { return load_8bit_mem(registers.A, registers.DE, 8); }

// 0x13: Increment reg-DE
int inc_de(void) { return inc_16bit_p(&registers.DE, 8); }

// 0x14: Increment reg-D
int inc_d(void) { return inc_8bit_p(&registers.D, 4); }

// 0x15: Decrement reg-D
int dec_d(void) { return dec_8bit_reg(&registers.D, 4); }

// 0x16: Load from memory(n) to reg-D
int ld_d_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.D, 8); }

// 0x17: Rotate reg-A left (+ old C-flag), set C-flag with MSB
int rla(void) {
	unsigned char regA_msb = registers.A >> 7;

	registers.A <<= 1;

	if (is_flag_set(CARRY))
		registers.A += 1;

	if (regA_msb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(ZERO);

	return 4;
}

// 0x18: Jump to current address + memory(n)
int jr_n(void) { return jump_n(); }

// 0x19: Add reg-DE to reg-HL
int add_hl_de(void) { return add_16bit_hl(registers.DE, 8); }

// 0x1A: Load from memory(DE) to reg-A
int ld_a_de(void) { return load_8bit_reg(read_byte(registers.DE), &registers.A, 8); }

// 0x1B: Decrement reg-DE
int dec_de(void) { return dec_16bit_reg(&registers.DE, 8); }

// 0x1C: Increment reg-E
int inc_e(void) { return inc_8bit_p(&registers.E, 4); }

// 0x1D: Decrement reg-E
int dec_e(void) { return dec_8bit_reg(&registers.E, 4); }

// 0x1E: Load from memory(n) to reg-E
int ld_e_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.E, 8); }

// 0x1F: Rotate reg-A right (+ old C-flag), set C-flag with LSB
int rra(void) {
	unsigned char regA_lsb = registers.A << 7;

	registers.A >>= 1;

	if (is_flag_set(CARRY))
		registers.A |= 0x80;

	if (regA_lsb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(ZERO);

	return 4;
}

// 0x20: Jump to current address + memory(n) if Z-flag is not set
int jr_nz_n(void) {
	if (!is_flag_set(ZERO))
		return jump_n() + 4;
	else {
		registers.PC++;
		return 8;
	}
}

// 0x21: Load from memory(nn) to reg-HL
int ld_hl_nn(void) { return load_16bit_nn_reg(&registers.HL, 12); }

// 0x22: Load from reg-A to memory(HL), increment reg-HL
int ldi_hl_a(void) { return load_8bit_mem(registers.A, registers.HL++, 8); }

// 0x23: Increment reg-HL
int inc_hl(void) { return inc_16bit_p(&registers.HL, 8); }

// 0x24: Increment reg-H
int inc_h(void) { return inc_8bit_p(&registers.H, 4); }

// 0x25: Decrement reg-H
int dec_h(void) { return dec_8bit_reg(&registers.H, 4); }

// 0x26: Load from memory(n) to reg-H
int ld_h_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.H, 8); }

// 0x27: Decimal Adjust reg-A
int daa(void) {
	if (!is_flag_set(NEGATIVE)) {
		if (is_flag_set(CARRY) || registers.A > 0x99) {
			registers.A += 0x60;
			set_flag(CARRY);
		}
		if (is_flag_set(HALFCARRY) || (registers.A & 0x09) > 0x09)
			registers.A += 0x06;
	}
	else {
		if (is_flag_set(CARRY))
			registers.A -= 0x60;
		if (is_flag_set(HALFCARRY))
			registers.A -= 0x06;
	}

	if (registers.A)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(HALFCARRY);

	return 4;
}

// 0x28: Jump to current address + memory(n) if Z-flag is set
int jr_z_n(void) {
	if (is_flag_set(ZERO))
		return jump_n() + 4;
	else {
		registers.PC++;
		return 8;
	}
}

// 0x29: Add reg-HL to reg-HL
int add_hl_hl(void) { return add_16bit_hl(registers.HL, 8); }

// 0x2A: Load from memory(HL) to reg-A, increment reg-HL
int ldi_a_hl(void) { return load_8bit_reg(read_byte(registers.HL++), &registers.A, 8); }

// 0x2B: Decrement reg-HL
int dec_hl(void) { return dec_16bit_reg(&registers.HL, 8); }

// 0x2C: Increment reg-L
int inc_l(void) { return inc_8bit_p(&registers.L, 4); }

// 0x2D: Decrement reg-L
int dec_l(void) { return dec_8bit_reg(&registers.L, 4); }

// 0x2E: Load from memory(n) to reg-L
int ld_l_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.L, 8); }

// 0x2F: Complement reg-A
int cpl_a(void) {
	registers.A = ~registers.A;

	set_flag(NEGATIVE);
	set_flag(HALFCARRY);

	return 4;
}

// 0x30: Jump to current address + memory(n) if C-flag is not set
int jr_nc_n(void) {
	if (!is_flag_set(CARRY))
		return jump_n() + 4;
	else {
		registers.PC++;
		return 8;
	}
}

// 0x31: Load from memory(nn) to reg-SP
int ld_sp_nn(void) { return load_16bit_nn_reg(&registers.SP, 12); }

// 0x32: Load from reg-A to memory(HL), decrement reg-HL
int ldd_hl_a(void) { return load_8bit_mem(registers.A, registers.HL--, 8); }

// 0x33: Increment reg-SP
int inc_sp(void) { return inc_16bit_p(&registers.SP, 8); }

// 0x34: Increment memory(HL)
int inc_hlp(void) { return inc_8bit_a(registers.HL, 12); }

// 0x35: Decrement memory(HL)
int dec_hlp(void) { return dec_8bit_mem(registers.HL, 12); }

// 0x36: Load from memory(n) to memory(HL)
int ld_hl_n(void) { return load_8bit_mem(read_byte(registers.PC++), registers.HL, 12); }

// 0x37: Set C-flag
int scf(void) {
	set_flag(CARRY);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 4;
}

// 0x38: Jump to current address + memory(n) if C-flag is set
int jr_c_n(void) {
	if (is_flag_set(CARRY))
		return jump_n() + 4;
	else {
		registers.PC++;
		return 8;
	}
}

// 0x39: Add reg-SP to reg-HL
int add_hl_sp(void) { return add_16bit_hl(registers.SP, 8); }

// 0x3A: Load from memory(HL) to reg-A, decrement reg-HL
int ldd_a_hl(void) { return load_8bit_reg(read_byte(registers.HL--), &registers.A, 8); }

// 0x3B: Decrement reg-SP
int dec_sp(void) { return dec_16bit_reg(&registers.SP, 8); }

// 0x3C: Increment reg-A
int inc_a(void) { return inc_8bit_p(&registers.A, 4); }

// 0x3D: Decrement reg-A
int dec_a(void) { return dec_8bit_reg(&registers.A, 4); }

// 0x3E: Load from memory(n) to reg-A
int ld_a_n(void) { return load_8bit_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0x3F: Complement C-flag
int ccf(void) {
	if (is_flag_set(CARRY))
		reset_flag(CARRY);
	else
		set_flag(CARRY);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 4;
}

// 0x40: Load from reg-B to reg-B
int ld_b_b(void) { return 4; }

// 0x41: Load from reg-C to reg-B
int ld_b_c(void) { return load_8bit_reg(registers.C, &registers.B, 4); }

// 0x42: Load from reg-D to reg-B
int ld_b_d(void) { return load_8bit_reg(registers.D, &registers.B, 4); }

// 0x43: Load from reg-E to reg-B
int ld_b_e(void) { return load_8bit_reg(registers.E, &registers.B, 4); }

// 0x44: Load from reg-H to reg-B
int ld_b_h(void) { return load_8bit_reg(registers.H, &registers.B, 4); }

// 0x45: Load from reg-L to reg-B
int ld_b_l(void) { return load_8bit_reg(registers.L, &registers.B, 4); }

// 0x46: Load from memory(HL) to reg-B
int ld_b_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.B, 8); }

// 0x47: Load from reg-A to reg-B
int ld_b_a(void) { return load_8bit_reg(registers.A, &registers.B, 4); }

// 0x48: Load from reg-B to reg-C
int ld_c_b(void) { return load_8bit_reg(registers.B, &registers.C, 4); }

// 0x49: Load from reg-C to reg-C
int ld_c_c(void) { return 4; }

// 0x4A: Load from reg-D to reg-C
int ld_c_d(void) { return load_8bit_reg(registers.D, &registers.C, 4); }

// 0x4B: Load from reg-E to reg-C
int ld_c_e(void) { return load_8bit_reg(registers.E, &registers.C, 4); }

// 0x4C: Load from reg-H to reg-C
int ld_c_h(void) { return load_8bit_reg(registers.H, &registers.C, 4); }

// 0x4D: Load from reg-B to reg-C
int ld_c_l(void) { return load_8bit_reg(registers.L, &registers.C, 4); }

// 0x4E: Load from memory(HL) to reg-C
int ld_c_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.C, 8); }

// 0x4F: Load from reg-A to reg-C
int ld_c_a(void) { return load_8bit_reg(registers.A, &registers.C, 4); }

// 0x50: Load from reg-B to reg-D
int ld_d_b(void) { return load_8bit_reg(registers.B, &registers.D, 4); }

// 0x51: Load from reg-C to reg-D
int ld_d_c(void) { return load_8bit_reg(registers.C, &registers.D, 4); }

// 0x52: Load from reg-D to reg-D
int ld_d_d(void) { return 4; }

// 0x53: Load from reg-E to reg-D
int ld_d_e(void) { return load_8bit_reg(registers.E, &registers.D, 4); }

// 0x54: Load from reg-H to reg-D
int ld_d_h(void) { return load_8bit_reg(registers.H, &registers.D, 4); }

// 0x55: Load from reg-L to reg-D
int ld_d_l(void) { return load_8bit_reg(registers.L, &registers.D, 4); }

// 0x56: Load from memory(HL) to reg-D
int ld_d_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.D, 8); }

// 0x57: Load from reg-A to reg-D
int ld_d_a(void) { return load_8bit_reg(registers.A, &registers.D, 4); }

// 0x58: Load from reg-B to reg-E
int ld_e_b(void) { return load_8bit_reg(registers.B, &registers.E, 4); }

// 0x59: Load from reg-C to reg-E
int ld_e_c(void) { return load_8bit_reg(registers.C, &registers.E, 4); }

// 0x5A: Load from reg-D to reg-E
int ld_e_d(void) { return load_8bit_reg(registers.D, &registers.E, 4); }

// 0x5B: Load from reg-E to reg-E
int ld_e_e(void) { return 4; }

// 0x5C: Load from reg-H to reg-E
int ld_e_h(void) { return load_8bit_reg(registers.H, &registers.E, 4); }

// 0x5D: Load from reg-L to reg-E
int ld_e_l(void) { return load_8bit_reg(registers.L, &registers.E, 4); }

// 0x5E: Load from memory(HL) to reg-E
int ld_e_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.E, 8); }

// 0x5F: Load from reg-A to reg-E
int ld_e_a(void) { return load_8bit_reg(registers.A, &registers.E, 4); }

// 0x60: Load from reg-B to reg-H
int ld_h_b(void) { return load_8bit_reg(registers.B, &registers.H, 4); }

// 0x61: Load from reg-C to reg-H
int ld_h_c(void) { return load_8bit_reg(registers.C, &registers.H, 4); }

// 0x62: Load from reg-D to reg-H
int ld_h_d(void) { return load_8bit_reg(registers.D, &registers.H, 4); }

// 0x63: Load from reg-E to reg-H
int ld_h_e(void) { return load_8bit_reg(registers.E, &registers.H, 4); }

// 0x64: Load from reg-H to reg-H
int ld_h_h(void) { return 4; }

// 0x65: Load from reg-L to reg-H
int ld_h_l(void) { return load_8bit_reg(registers.L, &registers.H, 4); }

// 0x66: Load from memory(HL) to reg-H
int ld_h_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.H, 8); }

// 0x67: Load from reg-A to reg-H
int ld_h_a(void) { return load_8bit_reg(registers.A, &registers.H, 4); }

// 0x68: Load from reg-B to reg-L
int ld_l_b(void) { return load_8bit_reg(registers.B, &registers.L, 4); }

// 0x69: Load from reg-C to reg-L
int ld_l_c(void) { return load_8bit_reg(registers.C, &registers.L, 4); }

// 0x6A: Load from reg-D to reg-L
int ld_l_d(void) { return load_8bit_reg(registers.D, &registers.L, 4);; }

// 0x6B: Load from reg-E to reg-L
int ld_l_e(void) { return load_8bit_reg(registers.E, &registers.L, 4); }

// 0x6C: Load from reg-H to reg-L
int ld_l_h(void) { return load_8bit_reg(registers.H, &registers.L, 4); }

// 0x6D: Load from reg-L to reg-L
int ld_l_l(void) { return 4; }

// 0x6E: Load from memory(HL) to reg-L
int ld_l_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.L, 8); }

// 0x6F: Load from reg-A to reg-L
int ld_l_a(void) { return load_8bit_reg(registers.A, &registers.L, 4); }

// 0x70: Load from reg-B to memory(HL)
int ld_hl_b(void) { return load_8bit_mem(registers.B, registers.HL, 8); }

// 0x71: Load from reg-C to memory(HL)
int ld_hl_c(void) { return load_8bit_mem(registers.C, registers.HL, 8); }

// 0x72: Load from reg-D to memory(HL)
int ld_hl_d(void) { return load_8bit_mem(registers.D, registers.HL, 8); }

// 0x73: Load from reg-E to memory(HL)
int ld_hl_e(void) { return load_8bit_mem(registers.E, registers.HL, 8); }

// 0x74: Load from reg-H to memory(HL)
int ld_hl_h(void) { return load_8bit_mem(registers.H, registers.HL, 8); }

// 0x75: Load from reg-L to memory(HL)
int ld_hl_l(void) { return load_8bit_mem(registers.L, registers.HL, 8); }

// 0x76: HALT Interrupt
int halt(void) {
	if (!interrupt_master_enable)
		registers.PC++;

	return 4;
}

// 0x77: Load from reg-A to memory(HL)
int ld_hl_a(void) { return load_8bit_mem(registers.A, registers.HL, 8); }

// 0x78: Load from reg-B to reg-A
int ld_a_b(void) { return load_8bit_reg(registers.B, &registers.A, 4); }

// 0x79: Load from reg-C to reg-A
int ld_a_c(void) { return load_8bit_reg(registers.C, &registers.A, 4); }

// 0x7A: Load from reg-D to reg-A
int ld_a_d(void) { return load_8bit_reg(registers.D, &registers.A, 4); }

// 0x7B: Load from reg-E to reg-A
int ld_a_e(void) { return load_8bit_reg(registers.E, &registers.A, 4); }

// 0x7C: Load from reg-H to reg-A
int ld_a_h(void) { return load_8bit_reg(registers.H, &registers.A, 4); }

// 0x7D: Load from reg-L to reg-A
int ld_a_l(void) { return load_8bit_reg(registers.L, &registers.A, 4); }

// 0x7E: Load from memory(HL) to reg-A
int ld_a_hl(void) { return load_8bit_reg(read_byte(registers.HL), &registers.A, 8); }

// 0x7F: Load from reg-A to reg-A
int ld_a_a(void) { return 4; }

// 0x80: Add reg-B to reg-A
int add_a_b(void) { return add_8bit_reg(registers.B, &registers.A, 4); }

// 0x81: Add reg-C to reg-A
int add_a_c(void) { return add_8bit_reg(registers.C, &registers.A, 4); }

// 0x82: Add reg-D to reg-A
int add_a_d(void) { return add_8bit_reg(registers.D, &registers.A, 4); }

// 0x83: Add reg-E to reg-A
int add_a_e(void) { return add_8bit_reg(registers.E, &registers.A, 4); }

// 0x84: Add reg-H to reg-A
int add_a_h(void) { return add_8bit_reg(registers.H, &registers.A, 4); }

// 0x85: Add reg-L to reg-A
int add_a_l(void) { return add_8bit_reg(registers.L, &registers.A, 4); }

// 0x86: Add memory(HL) to reg-A
int add_a_hl(void) { return add_8bit_reg(read_byte(registers.HL), &registers.A, 8); }

// 0x87: Add reg-A to reg-A
int add_a_a(void) { return add_8bit_reg(registers.A, &registers.A, 4); }

// 0x88: Add reg-B (+ C-flag) to reg-A
int adc_a_b(void) { return adc_8bit_reg(registers.B, &registers.A, 4); }

// 0x89: Add reg-C (+ C-flag) to reg-A
int adc_a_c(void) { return adc_8bit_reg(registers.C, &registers.A, 4); }

// 0x8A: Add reg-D (+ C-flag) to reg-A
int adc_a_d(void) { return adc_8bit_reg(registers.D, &registers.A, 4); }

// 0x8B: Add reg-E (+ C-flag) to reg-A
int adc_a_e(void) { return adc_8bit_reg(registers.E, &registers.A, 4); }

// 0x8C: Add reg-H (+ C-flag) to reg-A
int adc_a_h(void) { return adc_8bit_reg(registers.H, &registers.A, 4); }

// 0x8D: Add reg-L (+ C-flag) to reg-A
int adc_a_l(void) { return adc_8bit_reg(registers.L, &registers.A, 4); }

// 0x8E: Add memory(HL) (+ C-flag) to reg-A
int adc_a_hl(void) { return adc_8bit_reg(read_byte(registers.HL), &registers.A, 8); }

// 0x8F: Add reg-A (+ C-flag) to reg-A
int adc_a_a(void) { return adc_8bit_reg(registers.A, &registers.A, 4); }

// 0x90: Subtract reg-B from reg-A
int sub_a_b(void) { return sub_reg(registers.B, &registers.A, 4); }

// 0x91: Subtract reg-C from reg-A
int sub_a_c(void) { return sub_reg(registers.C, &registers.A, 4); }

// 0x92: Subtract reg-D from reg-A
int sub_a_d(void) { return sub_reg(registers.D, &registers.A, 4); }

// 0x93: Subtract reg-E from reg-A
int sub_a_e(void) { return sub_reg(registers.E, &registers.A, 4); }

// 0x94: Subtract reg-H from reg-A
int sub_a_h(void) { return sub_reg(registers.H, &registers.A, 4); }

// 0x95: Subtract reg-L from reg-A
int sub_a_l(void) { return sub_reg(registers.L, &registers.A, 4); }

// 0x96: Subtract memory(HL) from reg-A
int sub_a_hl(void) { return sub_reg(read_byte(registers.HL), &registers.A, 8); }

// 0x97: Subtract reg-A from reg-A
int sub_a_a(void) { return sub_reg(registers.A, &registers.A, 4); }

// 0x98: Subtract reg-B (+ C-flag) from reg-A
int sbc_a_b(void) { return sbc_reg(registers.B, &registers.A, 4); }

// 0x99: Subtract reg-C (+ C-flag) from reg-A
int sbc_a_c(void) { return sbc_reg(registers.C, &registers.A, 4); }

// 0x9A: Subtract reg-D (+ C-flag) from reg-A
int sbc_a_d(void) { return sbc_reg(registers.D, &registers.A, 4); }

// 0x9B: Subtract reg-E (+ C-flag) from reg-A
int sbc_a_e(void) { return sbc_reg(registers.E, &registers.A, 4); }

// 0x9C: Subtract reg-H (+ C-flag) from reg-A
int sbc_a_h(void) { return sbc_reg(registers.H, &registers.A, 4); }

// 0x9D: Subtract reg-L (+ C-flag) from reg-A
int sbc_a_l(void) { return sbc_reg(registers.L, &registers.A, 4); }

// 0x9E: Subtract memory(HL) (+ C-flag) from reg-A
int sbc_a_hl(void) { return sbc_reg(read_byte(registers.HL), &registers.A, 8); }

// 0x9F: Subtract reg-A (+ C-flag) from reg-A
int sbc_a_a(void) { return sbc_reg(registers.A, &registers.A, 4); }

// 0xA0: Logical AND, reg-B & reg-A, result in reg-A
int and_a_b(void) { return and_reg(registers.B, &registers.A, 4); }

// 0xA1: Logical AND, reg-C & reg-A, result in reg-A
int and_a_c(void) { return and_reg(registers.C, &registers.A, 4); }

// 0xA2: Logical AND, reg-D & reg-A, result in reg-A
int and_a_d(void) { return and_reg(registers.D, &registers.A, 4); }

// 0xA3: Logical AND, reg-E & reg-A, result in reg-A
int and_a_e(void) { return and_reg(registers.E, &registers.A, 4); }

// 0xA4: Logical AND, reg-H & reg-A, result in reg-A
int and_a_h(void) { return and_reg(registers.H, &registers.A, 4); }

// 0xA5: Logical AND, reg-L & reg-A, result in reg-A
int and_a_l(void) { return and_reg(registers.L, &registers.A, 4); }

// 0xA6: Logical AND, memory(HL) & reg-A, result in reg-A
int and_a_hl(void) { return and_reg(read_byte(registers.HL), &registers.A, 8); }

// 0xA7: Logical AND, reg-A & reg-A, result in reg-A
int and_a_a(void) { return and_reg(registers.A, &registers.A, 4); }

// 0xA8: Logical XOR, reg-B ^ reg-A, result in reg-A
int xor_a_b(void) { return xor_reg(registers.B, &registers.A, 4); }

// 0xA9: Logical XOR, reg-C ^ reg-A, result in reg-A
int xor_a_c(void) { return xor_reg(registers.C, &registers.A, 4); }

// 0xAA: Logical XOR, reg-D ^ reg-A, result in reg-A
int xor_a_d(void) { return xor_reg(registers.D, &registers.A, 4); }

// 0xAB: Logical XOR, reg-E ^ reg-A, result in reg-A
int xor_a_e(void) { return xor_reg(registers.E, &registers.A, 4); }

// 0xAC: Logical XOR, reg-H ^ reg-A, result in reg-A
int xor_a_h(void) { return xor_reg(registers.H, &registers.A, 4); }

// 0xAD: Logical XOR, reg-L ^ reg-A, result in reg-A
int xor_a_l(void) { return xor_reg(registers.L, &registers.A, 4); }

// 0xAE: Logical XOR, memory(HL) ^ reg-A, result in reg-A
int xor_a_hl(void) { return xor_reg(read_byte(registers.HL), &registers.A, 8); }

// 0xAF: Logical XOR, reg-A ^ reg-A, result in reg-A
int xor_a_a(void) { return xor_reg(registers.A, &registers.A, 4); }

// 0xB0: Logical OR, reg-B | reg-A, result in reg-A
int or_a_b(void) { return or_reg(registers.B, &registers.A, 4); }

// 0xB1: Logical OR, reg-C | reg-A, result in reg-A
int or_a_c(void) { return or_reg(registers.C, &registers.A, 4); }

// 0xB2: Logical OR, reg-D | reg-A, result in reg-A
int or_a_d(void) { return or_reg(registers.D, &registers.A, 4); }

// 0xB3: Logical OR, reg-E | reg-A, result in reg-A
int or_a_e(void) { return or_reg(registers.E, &registers.A, 4); }

// 0xB4: Logical OR, reg-H | reg-A, result in reg-A
int or_a_h(void) { return or_reg(registers.H, &registers.A, 4); }

// 0xB5: Logical OR, reg-L | reg-A, result in reg-A
int or_a_l(void) { return or_reg(registers.L, &registers.A, 4); }

// 0xB6: Logical OR, memory(HL) | reg-A, result in reg-A
int or_a_hl(void) { return or_reg(read_byte(registers.HL), &registers.A, 8); }

// 0xB7: Logical OR, reg-A | reg-A, result in reg-A
int or_a_a(void) { return or_reg(registers.A, &registers.A, 4); }

// 0xB8: Compare reg-B with reg-A
int cp_a_b(void) { return cp_reg(registers.B, &registers.A, 4); }

// 0xB9: Compare reg-C with reg-A
int cp_a_c(void) { return cp_reg(registers.C, &registers.A, 4); }

// 0xBA: Compare reg-D with reg-A
int cp_a_d(void) { return cp_reg(registers.D, &registers.A, 4); }

// 0xBB: Compare reg-E with reg-A
int cp_a_e(void) { return cp_reg(registers.E, &registers.A, 4); }

// 0xBC: Compare reg-H with reg-A
int cp_a_h(void) { return cp_reg(registers.H, &registers.A, 4); }

// 0xBD: Compare reg-L with reg-A
int cp_a_l(void) { return cp_reg(registers.L, &registers.A, 4); }

// 0xBE: Compare memory(HL) with reg-A
int cp_a_hl(void) { return cp_reg(read_byte(registers.HL), &registers.A, 8); }

// 0xBF: Compare reg-A with reg-A
int cp_a_a(void) { return cp_reg(registers.A, &registers.A, 4); }

// 0xC0: Pop two bytes from stack and jump to that address if Z-flag is not set
int ret_nz(void) {
	if (!is_flag_set(ZERO))
		return ret() + 12;
	else
		return 8;
}

// 0xC1: Pop from stack to reg-BC, increment SP twice
int pop_bc(void) {
	registers.BC = pop_short_stack();
	return 12;
}

// 0xC2: Jump to address in memory(nn) if Z-flag is not set
int jp_nz_nn(void) {
	if (!is_flag_set(ZERO))
		return jump_nn() + 4;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xC3: Jump to address in memory(nn)
int jp_nn(void) { return jump_nn(); }

// 0xC4: Push next instruction address to stack and jump to address in memory(nn) if Z-flag is not set
int call_nz_nn(void) {
	if (!is_flag_set(ZERO))
		return call_nn() + 12;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xC5: Push reg-BC to stack, decrement SP twice
int push_bc(void) {
	push_short_stack(registers.BC);
	return 16;
}

// 0xC6: Add memory(n) to reg-A
int add_a_n(void) { return add_8bit_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xC7: Push current address to stack and jump to address 0x0000
int rst_0(void) { return restart(0x0000); }

// 0xC8: Pop two bytes from stack and jump to that address if Z-flag is set
int ret_z(void) {
	if (is_flag_set(ZERO))
		return ret() + 12;
	else
		return 8;
}

// 0xC9: Pop two bytes from stack and jump to that address
int ret(void) {
	registers.PC = pop_short_stack();

	return 8;
}

// 0xCA: Jump to address in memory(nn) if Z-flag is set
int jp_z_nn(void) {
	if (is_flag_set(ZERO))
		return jump_nn() + 4;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xCB: Execute CB instructions
int cb(void) { return execute_cb_instruction(); }

// 0xCC: Push next instruction address to stack and jump to address in memory(nn) if Z-flag is set
int call_z_nn(void) {
	if (is_flag_set(ZERO))
		return call_nn() + 12;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xCD: Push next instruction address to stack and jump to address in memory(nn)
int call_nn(void) {
	unsigned short address = read_short(registers.PC);
	registers.PC += 2;

	push_short_stack(registers.PC);

	registers.PC = address;

	return 12;
}

// 0xCE: Add memory(n) (+ C-flag) to reg-A
int adc_a_n(void) { return adc_8bit_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xCF: Push current address to stack and jump to address 0x0008
int rst_8(void) { return restart(0x0008); }

// 0xD0: Pop two bytes from stack and jump to that address if C-flag is not set
int ret_nc(void) {
	if (!is_flag_set(CARRY))
		return ret() + 12;
	else
		return 8;
}

// 0xD1: Pop from stack to reg-DE, increment SP twice
int pop_de(void) {
	registers.DE = pop_short_stack();
	return 12;
}

// 0xD2: Jump to address in memory(nn) if C-flag is not set
int jp_nc_nn(void) {
	if (!is_flag_set(CARRY))
		return jump_nn() + 4;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xD4: Push next instruction address to stack and jump to address in memory(nn) if C-flag is not set
int call_nc_nn(void) {
	if (!is_flag_set(CARRY))
		return call_nn() + 12;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xD5: Push reg-DE to stack, decrement SP twice
int push_de(void) {
	push_short_stack(registers.DE);
	return 16;
}

// 0xD6: Subtract memory(n) from reg-A
int sub_a_n(void) { return sub_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xD7: Push current address to stack and jump to address 0x0010
int rst_10(void) { return restart(0x0010); }

// 0xD8: Pop two bytes from stack and jump to that address if C-flag is set
int ret_c(void) {
	if (is_flag_set(CARRY))
		return ret() + 12;
	else
		return 8;
}

// 0xD9: Pop two bytes from stack and jump to that address, then enable interrupts
int reti(void) {
	ret();

	interrupt_master_enable = true;

	return 8;
}

// 0xDA: Jump to address in memory(nn) if C-flag is set
int jp_c_nn(void) {
	if (is_flag_set(CARRY))
		return jump_nn() + 4;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xDC: Push next instruction address to stack and jump to address in memory(nn) if C-flag is set
int call_c_nn(void) {
	if (is_flag_set(CARRY))
		return call_nn() + 12;
	else {
		registers.PC += 2;
		return 12;
	}
}

// 0xDE: Subtract memory(n) (+ C-flag) from reg-A
int sbc_a_n(void) { return sbc_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xDF: Push current address to stack and jump to address 0x0018
int rst_18(void) { return restart(0x0018); }

// 0xE0: Load from reg-A to memory(0xFF00 + n)
int ld_ff_n_a(void) { return load_8bit_mem(registers.A, 0xFF00 + read_byte(registers.PC++), 12); }

// 0xE1: Pop from stack to reg-HL, increment SP twice
int pop_hl(void) {
	registers.HL = pop_short_stack();
	return 12;
}

// 0xE2: Load from reg-A to memory(0xFF00 + reg-C)
int ld_ff_c_a(void) { return load_8bit_mem(registers.A, 0xFF00 + registers.C, 8); }

// 0xE5: Push reg-HL to stack, decrement SP twice
int push_hl(void) {
	push_short_stack(registers.HL);
	return 16;
}

// 0xE6: Logical AND, memory(n) & reg-A, result in reg-A
int and_a_n(void) { return and_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xE7: Push current address to stack and jump to address 0x0020
int rst_20(void) { return restart(0x0020); }

// 0xE8: Add memory(n) to reg-SP
int add_sp_n(void) {
	unsigned char value = read_byte(registers.PC++);
	unsigned long result = registers.SP + value;

	if ((registers.SP & 0xFF + value) > 0xFF)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if ((registers.SP & 0x0F + value & 0x0F) > 0x0F)
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	registers.SP = (unsigned short)(result & 0xFFFF);

	reset_flag(ZERO);
	reset_flag(NEGATIVE);

	return 16;
}

// 0xE9: Jump to address in reg-HL
int jp_hl(void) {
	registers.PC = registers.HL;
	return 4;
}

// 0xEA: Load from reg-A to memory address pointed in memory(nn)
int ld_nnp_a(void) {
	write_byte(read_short(registers.PC), registers.A);
	registers.PC += 2;
	return 16;
}

// OxEE: Logical XOR, memory(n) ^ reg-A, result in reg-A
int xor_a_n(void) { return xor_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xEF: Push current address to stack and jump to address 0x0028
int rst_28(void) { return restart(0x0028); }

// 0xF0: Load from memory(0xFF00 + n) to reg-A
int ld_a_ff_n(void) {
	return load_8bit_reg(read_byte(0xFF00 + read_byte(registers.PC++)), &registers.A, 12);
}

// 0xF1: Pop from stack to reg-AF, increment SP twice
int pop_af(void) {
	registers.AF = pop_short_stack();
	return 12;
}

// 0xF2: Load from memory(0xFF00 + reg-C) to reg-A
int ld_a_ff_c(void) { return load_8bit_reg(read_byte(0xFF00 + registers.C), &registers.A, 8); }

// 0xF3: Disable interrupts
int di(void) {
	interrupt_master_enable = false;
	return 4;
}

// 0xF5: Push reg-AF to stack, decrement SP twice
int push_af(void) {
	push_short_stack(registers.AF);
	return 16;
}

// 0xF6: Logical OR, memory(n) | reg-A, result in reg-A
int or_a_n(void) { return or_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xF7: Push current address to stack and jump to address 0x0030
int rst_30(void) { return restart(0x0030); }

// 0xF8: Load from reg-SP + (signed)memory(n) to reg-HL
int ld_hl_sp_n(void) {
	signed char operand = (signed char)read_byte(registers.PC++);
	int addition = registers.SP + operand;

	if(addition > 0xFFFF)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	int low_nibble_add = (registers.SP & 0x0F) + (operand & 0x0F);
	if (low_nibble_add > 0x0F)
		set_flag(HALFCARRY);
	else
		reset_flag(HALFCARRY);

	reset_flag(ZERO);
	reset_flag(NEGATIVE);

	registers.HL = (unsigned short)(addition & 0xFFFF);

	return 12;
}

// 0xF9: Load from reg-HL to reg-SP
int ld_sp_hl(void) {
	registers.SP = registers.HL;
	return 8;
}

// 0xFA: Load from memory address pointed in memory(nn) to reg-A
int ld_a_nnp(void) {
	registers.A = read_byte(read_short(registers.PC));
	registers.PC += 2;
	return 16;
}

// 0xFB: Enable interrupts
int ei(void) {
	interrupt_master_enable = true;
	return 4;
}

// 0xFE: Compare memory(n) with reg-A
int cp_a_n(void) { return cp_reg(read_byte(registers.PC++), &registers.A, 8); }

// 0xFF: Push current address to stack and jump to address 0x0038
int rst_38(void) { return restart(0x0038); }

//endregion
