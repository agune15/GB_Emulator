#include <stdlib.h>
#include "cb.h"
#include "cb_instructions.h"
#include "registers.h"
#include "memory.h"
#include "cpu.h"

int (*cb_instructions[256])(void) = {
/*0x0*/	rlc_b, rlc_c, rlc_d, rlc_e, rlc_h, rlc_l, rlc_hl, rlc_a, rrc_b, rrc_c, rrc_d, rrc_e, rrc_h, rrc_l, rrc_hl, rrc_a,
/*0x1*/	rl_b, rl_c, rl_d, rl_e, rl_h, rl_l, rl_hl, rl_a, rr_b, rr_c, rr_d, rr_e, rr_h, rr_l, rr_hl, rr_a,
/*0x2*/	sla_b, sla_c, sla_d, sla_e, sla_h, sla_l, sla_hl, sla_a, sra_b, sra_c, sra_d, sra_e, sra_h, sra_l, sra_hl, sra_a,
/*0x3*/	swap_b, swap_c, swap_d, swap_e, swap_h, swap_l, swap_hl, swap_a, srl_b, srl_c, srl_d, srl_e, srl_h, srl_l, srl_hl, srl_a,
/*0x4*/	bit_0_b, bit_0_c, bit_0_d, bit_0_e, bit_0_h, bit_0_l, bit_0_hl, bit_0_a, bit_1_b, bit_1_c, bit_1_d, bit_1_e, bit_1_h, bit_1_l, bit_1_hl, bit_1_a,
/*0x5*/	bit_2_b, bit_2_c, bit_2_d, bit_2_e, bit_2_h, bit_2_l, bit_2_hl, bit_2_a, bit_3_b, bit_3_c, bit_3_d, bit_3_e, bit_3_h, bit_3_l, bit_3_hl, bit_3_a,
/*0x6*/	bit_4_b, bit_4_c, bit_4_d, bit_4_e, bit_4_h, bit_4_l, bit_4_hl, bit_4_a, bit_5_b, bit_5_c, bit_5_d, bit_5_e, bit_5_h, bit_5_l, bit_5_hl, bit_5_a,
/*0x7*/	bit_6_b, bit_6_c, bit_6_d, bit_6_e, bit_6_h, bit_6_l, bit_6_hl, bit_6_a, bit_7_b, bit_7_c, bit_7_d, bit_7_e, bit_7_h, bit_7_l, bit_7_hl, bit_7_a,
/*0x8*/	res_0_b, res_0_c, res_0_d, res_0_e, res_0_h, res_0_l, res_0_hl, res_0_a, res_1_b, res_1_c, res_1_d, res_1_e, res_1_h, res_1_l, res_1_hl, res_1_a,
/*0x9*/	res_2_b, res_2_c, res_2_d, res_2_e, res_2_h, res_2_l, res_2_hl, res_2_a, res_3_b, res_3_c, res_3_d, res_3_e, res_3_h, res_3_l, res_3_hl, res_3_a,
/*0xA*/	res_4_b, res_4_c, res_4_d, res_4_e, res_4_h, res_4_l, res_4_hl, res_4_a, res_5_b, res_5_c, res_5_d, res_5_e, res_5_h, res_5_l, res_5_hl, res_5_a,
/*0xB*/ res_6_b, res_6_c, res_6_d, res_6_e, res_6_h, res_6_l, res_6_hl, res_6_a, res_7_b, res_7_c, res_7_d, res_7_e, res_7_h, res_7_l, res_7_hl, res_7_a,
/*0xC*/	set_0_b, set_0_c, set_0_d, set_0_e, set_0_h, set_0_l, set_0_hl, set_0_a, set_1_b, set_1_c, set_1_d, set_1_e, set_1_h, set_1_l, set_1_hl, set_1_a,
/*0xD*/	set_2_b, set_2_c, set_2_d, set_2_e, set_2_h, set_2_l, set_2_hl, set_2_a, set_3_b, set_3_c, set_3_d, set_3_e, set_3_h, set_3_l, set_3_hl, set_3_a,
/*0xE*/	set_4_b, set_4_c, set_4_d, set_4_e, set_4_h, set_4_l, set_4_hl, set_4_a, set_5_b, set_5_c, set_5_d, set_5_e, set_5_h, set_5_l, set_5_hl, set_5_a,
/*0xF*/	set_6_b, set_6_c, set_6_d, set_6_e, set_6_h, set_6_l, set_6_hl, set_6_a, set_7_b, set_7_c, set_7_d, set_7_e, set_7_h, set_7_l, set_7_hl, set_7_a,
};

//TODO: Description
int execute_cb_instruction(void)
{
	unsigned char instruction = read_byte(registers.PC++);
	return (*cb_instructions[instruction])();
}

// Helpers

//region SWAP

// Swap low nibble with high nibble of register
int swap_8bit_p(unsigned char *reg, int cycles)
{
	unsigned char swapped_low_nib = (*reg << 4) & 0xF0;
	*reg = (*reg >> 4) & 0x0F;

	*reg |= swapped_low_nib;

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(CARRY);

	return cycles;
}

// Swap low nibble with high nibble from address
int swap_8bit_a(unsigned short address, int cycles)
{
	unsigned char byte = read_byte(address);

	unsigned char swapped_low_nib = (byte << 4) & 0xF0;
	byte = (byte >> 4) & 0x0F;

	byte |= swapped_low_nib;
	write_byte(address, byte);

	if (byte)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);
	reset_flag(CARRY);

	return cycles;
}

//endregion

//region Rotates

// Rotate reg left (+ new carry flag), set carry flag with MSB
int rlc(unsigned char *reg)
{
	unsigned char reg_msb = *reg >> 7;

	*reg <<= 1;
	*reg |= reg_msb;

	if (reg_msb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

// Rotate reg left (+ old carry flag), set carry flag with MSB
int rl(unsigned char *reg)
{
	unsigned char reg_msb = *reg >> 7;

	*reg <<= 1;

	if(is_flag_set(CARRY))
		*reg += 1;

	if(reg_msb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if(*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

// Rotate reg right (+ new carry flag), set carry flag with LSB
int rrc(unsigned char *reg) {
	unsigned char reg_lsb = *reg << 7;

	*reg >>= 1;
	*reg |= reg_lsb;

	if (reg_lsb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

// Rotate reg right (+ old carry flag), set carry flag with LSB
int rr(unsigned char *reg) {
	unsigned char reg_lsb = *reg << 7;

	*reg >>= 1;

	if (is_flag_set(CARRY))
		*reg |= 0x80;

	if (reg_lsb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (*reg)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

//endregion

//region Shifts

// Shift reg left into carry flag, set LSB to 0
int sla(unsigned char *reg)
{
	if (*reg & 0x80)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	*reg <<= 1;

	if (*reg)
		reset_flag(ZERO);
	else
		reset_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

// Shift reg right into carry flag, MSB does not change
int sra(unsigned char *reg)
{
	if (*reg & 0x01)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	*reg = (*reg & 0x80) | (*reg >> 1);

	if (*reg)
		reset_flag(ZERO);
	else
		reset_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

// Shift reg right into carry flag, set MSB to 0
int srl(unsigned char *reg)
{
	if (*reg & 0x01)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	*reg >>= 1;

	if (*reg)
		reset_flag(ZERO);
	else
		reset_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 8;
}

//endregion

//region Bit tests

// Test bit of reg
int test_bit(int bit_n, unsigned char *reg)
{
	if (*reg >> bit_n & 1)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	set_flag(HALFCARRY);

	return 8;
}

// Test bit of memory(HL)
int test_bit_hl(int bit_n)
{
	if (read_byte(registers.HL) >> bit_n & 1)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	set_flag(HALFCARRY);

	return 16;
}

//endregion

//region Bit sets

// Set bit of reg
int set_bit(int bit_n, unsigned char *reg)
{
	*reg |= 1 << bit_n;

	return 8;
}

// Set bit of memory(HL)
int set_bit_hl(int bit_n)
{
	write_byte(registers.HL, read_byte(registers.HL) | 1 << bit_n);

	return 16;
}

//endregion

//region Bit resets

// Reset bit of reg
int res_bit(int bit_n, unsigned char *reg)
{
	*reg &= ~(1 << bit_n);

	return 8;
}

// Reset bit of memory(HL)
int res_bit_hl(int bit_n)
{
	write_byte(registers.HL, read_byte(registers.HL) & ~(1 << bit_n));

	return 16;
}

//endregion

// CB Instructions

// 0x00: Rotate reg-B left (+ new carry flag), set carry flag with MSB
int rlc_b(void) { return rlc(&registers.B); }

// 0x01: Rotate reg-C left (+ new carry flag), set carry flag with MSB
int rlc_c(void) { return rlc(&registers.C); }

// 0x02: Rotate reg-D left (+ new carry flag), set carry flag with MSB
int rlc_d(void) { return rlc(&registers.D); }

// 0x03: Rotate reg-E left (+ new carry flag), set carry flag with MSB
int rlc_e(void) { return rlc(&registers.E); }

// 0x04: Rotate reg-H left (+ new carry flag), set carry flag with MSB
int rlc_h(void) { return rlc(&registers.H); }

// 0x05: Rotate reg-L left (+ new carry flag), set carry flag with MSB
int rlc_l(void) { return rlc(&registers.L); }

// 0x06: Rotate memory(HL) left (+ new carry flag), set carry flag with MSB
int rlc_hl(void) {
	unsigned char value = read_byte(registers.HL);
	unsigned char msb = value >> 7;

	value <<= 1;
	value |= msb;

	write_byte(registers.HL, value);

	if (msb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (value)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x07: Rotate reg-A left (+ new carry flag), set carry flag with MSB
int rlc_a(void) { return rlc(&registers.A); }

// 0x08: Rotate reg-B right (+ new carry flag), set carry flag with LSB
int rrc_b(void) { return rrc(&registers.B); }

// 0x09: Rotate reg-C right (+ new carry flag), set carry flag with LSB
int rrc_c(void) { return rrc(&registers.C); }

// 0x0A: Rotate reg-D right (+ new carry flag), set carry flag with LSB
int rrc_d(void) { return rrc(&registers.D); }

// 0x0B: Rotate reg-E right (+ new carry flag), set carry flag with LSB
int rrc_e(void) { return rrc(&registers.E); }

// 0x0C: Rotate reg-H right (+ new carry flag), set carry flag with LSB
int rrc_h(void) { return rrc(&registers.H); }

// 0x0D: Rotate reg-L right (+ new carry flag), set carry flag with LSB
int rrc_l(void) { return rrc(&registers.L); }

// 0x0E: Rotate memory(HL) right (+ new carry flag), set carry flag with LSB
int rrc_hl(void) {
	unsigned char value = read_byte(registers.HL);
	unsigned char lsb = value << 7;

	value >>= 1;
	value |= lsb;

	write_byte(registers.HL, value);

	if (lsb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (value)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x0F: Rotate reg-A right (+ new carry flag), set carry flag with LSB
int rrc_a(void) { return rrc(&registers.A); }

// 0x10: Rotate reg-B left (+ old carry flag), set carry flag with MSB
int rl_b(void) { return rl(&registers.B); }

// 0x11: Rotate reg-C left (+ old carry flag), set carry flag with MSB
int rl_c(void) { return rl(&registers.C); }

// 0x12: Rotate reg-D left (+ old carry flag), set carry flag with MSB
int rl_d(void) { return rl(&registers.D); }

// 0x13: Rotate reg-E left (+ old carry flag), set carry flag with MSB
int rl_e(void) { return rl(&registers.E); }

// 0x14: Rotate reg-H left (+ old carry flag), set carry flag with MSB
int rl_h(void) { return rl(&registers.H); }

// 0x15: Rotate reg-L left (+ old carry flag), set carry flag with MSB
int rl_l(void) { return rl(&registers.L); }

// 0x16: Rotate memory(HL) left (+ old carry flag), set carry flag with MSB
int rl_hl(void) {
	unsigned char value = read_byte(registers.HL);
	unsigned char msb = value >> 7;

	value <<= 1;

	if (is_flag_set(CARRY))
		value += 1;

	write_byte(registers.HL, value);

	if (msb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (value)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x17: Rotate reg-A left (+ old carry flag), set carry flag with MSB
int rl_a(void) { return rl(&registers.A); }

// 0x18: Rotate reg-B right (+ old carry flag), set carry flag with LSB
int rr_b(void) { return rr(&registers.B); }

// 0x19: Rotate reg-C right (+ old carry flag), set carry flag with LSB
int rr_c(void) { return rr(&registers.C); }

// 0x1A: Rotate reg-D right (+ old carry flag), set carry flag with LSB
int rr_d(void) { return rr(&registers.D); }

// 0x1B: Rotate reg-E right (+ old carry flag), set carry flag with LSB
int rr_e(void) { return rr(&registers.E); }

// 0x1C: Rotate reg-H right (+ old carry flag), set carry flag with LSB
int rr_h(void) { return rr(&registers.H); }

// 0x1D: Rotate reg-L right (+ old carry flag), set carry flag with LSB
int rr_l(void) { return rr(&registers.L); }

// 0x1E: Rotate memory(HL) right (+ old carry flag), set carry flag with LSB
int rr_hl(void) {
	unsigned char value = read_byte(registers.HL);
	unsigned char lsb = value << 7;

	value >>= 1;

	if (is_flag_set(CARRY))
		value |= 0x80;

	write_byte(registers.HL, value);

	if (lsb)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	if (value)
		reset_flag(ZERO);
	else
		set_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x1F: Rotate reg-A right (+ old carry flag), set carry flag with LSB
int rr_a(void) { return rr(&registers.A); }

// 0x20: Shift reg-B left into carry flag, set LSB to 0
int sla_b(void) { return sla(&registers.B); }

// 0x20: Shift reg-C left into carry flag, set LSB to 0
int sla_c(void) { return sla(&registers.C); }

// 0x22: Shift reg-D left into carry flag, set LSB to 0
int sla_d(void) { return sla(&registers.D); }

// 0x23: Shift reg-E left into carry flag, set LSB to 0
int sla_e(void) { return sla(&registers.E); }

// 0x24: Shift reg-H left into carry flag, set LSB to 0
int sla_h(void) { return sla(&registers.H); }

// 0x25: Shift reg-L left into carry flag, set LSB to 0
int sla_l(void) { return sla(&registers.L); }

// 0x26: Shift memory(HL) left into carry flag, set LSB to 0
int sla_hl(void) {
	unsigned char value = read_byte(registers.HL);

	if (value & 0x80)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	value <<= 1;

	write_byte(registers.HL, value);

	if (value)
		reset_flag(ZERO);
	else
		reset_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x27: Shift reg-A left into carry flag, set LSB to 0
int sla_a(void) { return sla(&registers.A); }

// 0x28: Shift reg-B right into carry flag, MSB does not change
int sra_b(void) { return sra(&registers.B); }

// 0x29: Shift reg-C right into carry flag, MSB does not change
int sra_c(void) { return sra(&registers.C); }

// 0x2A: Shift reg-D right into carry flag, MSB does not change
int sra_d(void) { return sra(&registers.D); }

// 0x2B: Shift reg-E right into carry flag, MSB does not change
int sra_e(void) { return sra(&registers.E); }

// 0x2C: Shift reg-H right into carry flag, MSB does not change
int sra_h(void) { return sra(&registers.H); }

// 0x2D: Shift reg-L right into carry flag, MSB does not change
int sra_l(void) { return sra(&registers.L); }

// 0x2E: Shift memory(HL) right into carry flag, MSB does not change
int sra_hl(void) {
	unsigned char value = read_byte(registers.HL);

	if (value & 0x01)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	value = (value & 0x80) | (value >> 1);

	write_byte(registers.HL, value);

	if (value)
		reset_flag(ZERO);
	else
		reset_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x2F: Shift reg-A right into carry flag, MSB does not change
int sra_a(void) { return sra(&registers.A); }

// 0x30: Swap reg-B
int swap_b(void) { return swap_8bit_p(&registers.B, 8); }

// 0x31: Swap reg-C
int swap_c(void) { return swap_8bit_p(&registers.C, 8); }

// 0x32: Swap reg-D
int swap_d(void) { return swap_8bit_p(&registers.D, 8); }

// 0x33: Swap reg-E
int swap_e(void) { return swap_8bit_p(&registers.E, 8); }

// 0x34: Swap reg-H
int swap_h(void) { return swap_8bit_p(&registers.H, 8); }

// 0x35: Swap reg-L
int swap_l(void) { return swap_8bit_p(&registers.L, 8); }

// 0x36: Swap memory(HL)
int swap_hl(void) { return swap_8bit_a(registers.HL, 10); }

// 0x37: Swap reg-A
int swap_a(void) { return swap_8bit_p(&registers.A, 8); }

// 0x38: Shift reg-B right into carry flag, set MSB to 0
int srl_b(void) { return srl(&registers.B); }

// 0x39: Shift reg-C right into carry flag, set MSB to 0
int srl_c(void) { return srl(&registers.C); }

// 0x3A: Shift reg-D right into carry flag, set MSB to 0
int srl_d(void) { return srl(&registers.D); }

// 0x3B: Shift reg-E right into carry flag, set MSB to 0
int srl_e(void) { return srl(&registers.E); }

// 0x3C: Shift reg-H right into carry flag, set MSB to 0
int srl_h(void) { return srl(&registers.H); }

// 0x3D: Shift reg-L right into carry flag, set MSB to 0
int srl_l(void) { return srl(&registers.L); }

// 0x3E: Shift memory(HL) right into carry flag, set MSB to 0
int srl_hl(void) {
	unsigned char value = read_byte(registers.HL);

	if (value & 0x01)
		set_flag(CARRY);
	else
		reset_flag(CARRY);

	value >>= 1;

	write_byte(registers.HL, value);

	if (value)
		reset_flag(ZERO);
	else
		reset_flag(ZERO);

	reset_flag(NEGATIVE);
	reset_flag(HALFCARRY);

	return 16;
}

// 0x3F: Shift reg-A right into carry flag, set MSB to 0
int srl_a(void) { return srl(&registers.A); }

// 0x40: Test bit 0 of reg-B
int bit_0_b(void) { return test_bit(0, &registers.B); }

// 0x41: Test bit 0 of reg-C
int bit_0_c(void) { return test_bit(0, &registers.C); }

// 0x42: Test bit 0 of reg-D
int bit_0_d(void) { return test_bit(0, &registers.D); }

// 0x43: Test bit 0 of reg-E
int bit_0_e(void) { return test_bit(0, &registers.E); }

// 0x44: Test bit 0 of reg-H
int bit_0_h(void) { return test_bit(0, &registers.H); }

// 0x45: Test bit 0 of reg-L
int bit_0_l(void) { return test_bit(0, &registers.L); }

// 0x46: Test bit 0 of memory(HL)
int bit_0_hl(void) { return test_bit_hl(0); }

// 0x47: Test bit 0 of reg-A
int bit_0_a(void) { return test_bit(0, &registers.A); }

// 0x48: Test bit 1 of reg-B
int bit_1_b(void) { return test_bit(1, &registers.B); }

// 0x49: Test bit 1 of reg-C
int bit_1_c(void) { return test_bit(1, &registers.C); }

// 0x4A: Test bit 1 of reg-D
int bit_1_d(void) { return test_bit(1, &registers.D); }

// 0x4B: Test bit 1 of reg-E
int bit_1_e(void) { return test_bit(1, &registers.E); }

// 0x4C: Test bit 1 of reg-H
int bit_1_h(void) { return test_bit(1, &registers.H); }

// 0x4D: Test bit 1 of reg-L
int bit_1_l(void) { return test_bit(1, &registers.L); }

// 0x4E: Test bit 1 of memory(HL)
int bit_1_hl(void) { return test_bit_hl(1); }

// 0x4F: Test bit 1 of reg-A
int bit_1_a(void) { return test_bit(1, &registers.A); }

// 0x50: Test bit 2 of reg-B
int bit_2_b(void) { return test_bit(2, &registers.B); }

// 0x51: Test bit 2 of reg-C
int bit_2_c(void) { return test_bit(2, &registers.C); }

// 0x52: Test bit 2 of reg-D
int bit_2_d(void) { return test_bit(2, &registers.D); }

// 0x53: Test bit 2 of reg-E
int bit_2_e(void) { return test_bit(2, &registers.E); }

// 0x54: Test bit 2 of reg-H
int bit_2_h(void) { return test_bit(2, &registers.H); }

// 0x55: Test bit 2 of reg-L
int bit_2_l(void) { return test_bit(2, &registers.L); }

// 0x56: Test bit 2 of memory(HL)
int bit_2_hl(void) { return test_bit_hl(2); }

// 0x57: Test bit 2 of reg-A
int bit_2_a(void) { return test_bit(2, &registers.A); }

// 0x58: Test bit 3 of reg-B
int bit_3_b(void) { return test_bit(3, &registers.B); }

// 0x59: Test bit 3 of reg-C
int bit_3_c(void) { return test_bit(3, &registers.C); }

// 0x5A: Test bit 3 of reg-D
int bit_3_d(void) { return test_bit(3, &registers.D); }

// 0x5B: Test bit 3 of reg-E
int bit_3_e(void) { return test_bit(3, &registers.E); }

// 0x5C: Test bit 3 of reg-H
int bit_3_h(void) { return test_bit(3, &registers.H); }

// 0x5D: Test bit 3 of reg-L
int bit_3_l(void) { return test_bit(3, &registers.L); }

// 0x5E: Test bit 3 of memory(HL)
int bit_3_hl(void) { return test_bit_hl(3); }

// 0x5F: Test bit 3 of reg-A
int bit_3_a(void) { return test_bit(3, &registers.A); }

// 0x60: Test bit 4 of reg-B
int bit_4_b(void) { return test_bit(4, &registers.B); }

// 0x61: Test bit 4 of reg-C
int bit_4_c(void) { return test_bit(4, &registers.C); }

// 0x62: Test bit 4 of reg-D
int bit_4_d(void) { return test_bit(4, &registers.D); }

// 0x63: Test bit 4 of reg-E
int bit_4_e(void) { return test_bit(4, &registers.E); }

// 0x64: Test bit 4 of reg-H
int bit_4_h(void) { return test_bit(4, &registers.H); }

// 0x65: Test bit 4 of reg-L
int bit_4_l(void) { return test_bit(4, &registers.L); }

// 0x66: Test bit 4 of memory(HL)
int bit_4_hl(void) { return test_bit_hl(4); }

// 0x67: Test bit 4 of reg-A
int bit_4_a(void) { return test_bit(4, &registers.A); }

// 0x68: Test bit 5 of reg-B
int bit_5_b(void) { return test_bit(5, &registers.B); }

// 0x69: Test bit 5 of reg-C
int bit_5_c(void) { return test_bit(5, &registers.C); }

// 0x6A: Test bit 5 of reg-D
int bit_5_d(void) { return test_bit(5, &registers.D); }

// 0x6B: Test bit 5 of reg-E
int bit_5_e(void) { return test_bit(5, &registers.E); }

// 0x6C: Test bit 5 of reg-H
int bit_5_h(void) { return test_bit(5, &registers.H); }

// 0x6D: Test bit 5 of reg-L
int bit_5_l(void) { return test_bit(5, &registers.L); }

// 0x6E: Test bit 5 of memory(HL)
int bit_5_hl(void) { return test_bit_hl(5); }

// 0x6F: Test bit 5 of reg-A
int bit_5_a(void) { return test_bit(5, &registers.A); }

// 0x70: Test bit 6 of reg-B
int bit_6_b(void) { return test_bit(6, &registers.B); }

// 0x71: Test bit 6 of reg-C
int bit_6_c(void) { return test_bit(6, &registers.C); }

// 0x72: Test bit 6 of reg-D
int bit_6_d(void) { return test_bit(6, &registers.D); }

// 0x73: Test bit 6 of reg-E
int bit_6_e(void) { return test_bit(6, &registers.E); }

// 0x74: Test bit 6 of reg-H
int bit_6_h(void) { return test_bit(6, &registers.H); }

// 0x75: Test bit 6 of reg-L
int bit_6_l(void) { return test_bit(6, &registers.L); }

// 0x76: Test bit 6 of memory(HL)
int bit_6_hl(void) { return test_bit_hl(6); }

// 0x77: Test bit 6 of reg-A
int bit_6_a(void) { return test_bit(6, &registers.A); }

// 0x78: Test bit 7 of reg-B
int bit_7_b(void) { return test_bit(7, &registers.B); }

// 0x79: Test bit 7 of reg-C
int bit_7_c(void) { return test_bit(7, &registers.C); }

// 0x7A: Test bit 7 of reg-D
int bit_7_d(void) { return test_bit(7, &registers.D); }

// 0x7B: Test bit 7 of reg-E
int bit_7_e(void) { return test_bit(7, &registers.E); }

// 0x7C: Test bit 7 of reg-H
int bit_7_h(void) { return test_bit(7, &registers.H); }

// 0x7D: Test bit 7 of reg-L
int bit_7_l(void) { return test_bit(7, &registers.L); }

// 0x7E: Test bit 7 of memory(HL)
int bit_7_hl(void) { return test_bit_hl(7); }

// 0x7F: Test bit 7 of reg-A
int bit_7_a(void) { return test_bit(7, &registers.A); }

// 0x80: Reset bit 0 of reg-B
int res_0_b(void) { return res_bit(0, &registers.B); }

// 0x81: Reset bit 0 of reg-C
int res_0_c(void) { return res_bit(0, &registers.C); }

// 0x82: Reset bit 0 of reg-D
int res_0_d(void) { return res_bit(0, &registers.D); }

// 0x83: Reset bit 0 of reg-E
int res_0_e(void) { return res_bit(0, &registers.E); }

// 0x84: Reset bit 0 of reg-H
int res_0_h(void) { return res_bit(0, &registers.H); }

// 0x85: Reset bit 0 of reg-L
int res_0_l(void) { return res_bit(0, &registers.L); }

// 0x86: Reset bit 0 of memory(HL)
int res_0_hl(void) { return res_bit_hl(0); }

// 0x87: Reset bit 0 of reg-A
int res_0_a(void) { return res_bit(0, &registers.A); }

// 0x88: Reset bit 1 of reg-B
int res_1_b(void) { return res_bit(1, &registers.B); }

// 0x89: Reset bit 1 of reg-C
int res_1_c(void) { return res_bit(1, &registers.C); }

// 0x8A: Reset bit 1 of reg-D
int res_1_d(void) { return res_bit(1, &registers.D); }

// 0x8B: Reset bit 1 of reg-E
int res_1_e(void) { return res_bit(1, &registers.E); }

// 0x8C: Reset bit 1 of reg-H
int res_1_h(void) { return res_bit(1, &registers.H); }

// 0x8D: Reset bit 1 of reg-L
int res_1_l(void) { return res_bit(1, &registers.L); }

// 0x8E: Reset bit 1 of memory(HL)
int res_1_hl(void) { return res_bit_hl(1); }

// 0x8F: Reset bit 1 of reg-A
int res_1_a(void) { return res_bit(1, &registers.A); }

// 0x90: Reset bit 2 of reg-B
int res_2_b(void) { return res_bit(2, &registers.B); }

// 0x91: Reset bit 2 of reg-C
int res_2_c(void) { return res_bit(2, &registers.C); }

// 0x92: Reset bit 2 of reg-D
int res_2_d(void) { return res_bit(2, &registers.D); }

// 0x93: Reset bit 2 of reg-E
int res_2_e(void) { return res_bit(2, &registers.E); }

// 0x94: Reset bit 2 of reg-H
int res_2_h(void) { return res_bit(2, &registers.H); }

// 0x95: Reset bit 2 of reg-L
int res_2_l(void) { return res_bit(2, &registers.L); }

// 0x96: Reset bit 2 of memory(HL)
int res_2_hl(void) { return res_bit_hl(2); }

// 0x97: Reset bit 2 of reg-A
int res_2_a(void) { return res_bit(2, &registers.A); }

// 0x98: Reset bit 3 of reg-B
int res_3_b(void) { return res_bit(3, &registers.B); }

// 0x99: Reset bit 3 of reg-C
int res_3_c(void) { return res_bit(3, &registers.C); }

// 0x9A: Reset bit 3 of reg-D
int res_3_d(void) { return res_bit(3, &registers.D); }

// 0x9B: Reset bit 3 of reg-E
int res_3_e(void) { return res_bit(3, &registers.E); }

// 0x9C: Reset bit 3 of reg-H
int res_3_h(void) { return res_bit(3, &registers.H); }

// 0x9D: Reset bit 3 of reg-L
int res_3_l(void) { return res_bit(3, &registers.L); }

// 0x9E: Reset bit 3 of memory(HL)
int res_3_hl(void) { return res_bit_hl(3); }

// 0x9F: Reset bit 3 of reg-A
int res_3_a(void) { return res_bit(3, &registers.A); }

// 0xA0: Reset bit 4 of reg-B
int res_4_b(void) { return res_bit(4, &registers.B); }

// 0xA1: Reset bit 4 of reg-C
int res_4_c(void) { return res_bit(4, &registers.C); }

// 0xA2: Reset bit 4 of reg-D
int res_4_d(void) { return res_bit(4, &registers.D); }

// 0xA3: Reset bit 4 of reg-E
int res_4_e(void) { return res_bit(4, &registers.E); }

// 0xA4: Reset bit 4 of reg-H
int res_4_h(void) { return res_bit(4, &registers.H); }

// 0xA5: Reset bit 4 of reg-L
int res_4_l(void) { return res_bit(4, &registers.L); }

// 0xA6: Set bit 4 of memory(HL)
int res_4_hl(void) { return res_bit_hl(4); }

// 0xA7: Reset bit 4 of reg-A
int res_4_a(void) { return res_bit(4, &registers.A); }

// 0xA8: Reset bit 5 of reg-B
int res_5_b(void) { return res_bit(5, &registers.B); }

// 0xA9: Reset bit 5 of reg-C
int res_5_c(void) { return res_bit(5, &registers.C); }

// 0xAA: Reset bit 5 of reg-D
int res_5_d(void) { return res_bit(5, &registers.D); }

// 0xAB: Reset bit 5 of reg-E
int res_5_e(void) { return res_bit(5, &registers.E); }

// 0xAC: Reset bit 5 of reg-H
int res_5_h(void) { return res_bit(5, &registers.H); }

// 0xAD: Reset bit 5 of reg-L
int res_5_l(void) { return res_bit(5, &registers.L); }

// 0xAE: Reset bit 5 of memory(HL)
int res_5_hl(void) { return res_bit_hl(5); }

// 0xAF: Reset bit 5 of reg-A
int res_5_a(void) { return res_bit(5, &registers.A); }

// 0xB0: Reset bit 6 of reg-B
int res_6_b(void) { return res_bit(6, &registers.B); }

// 0xB1: Reset bit 6 of reg-C
int res_6_c(void) { return res_bit(6, &registers.C); }

// 0xB2: Reset bit 6 of reg-D
int res_6_d(void) { return res_bit(6, &registers.D); }

// 0xB3: Reset bit 6 of reg-E
int res_6_e(void) { return res_bit(6, &registers.E); }

// 0xB4: Reset bit 6 of reg-H
int res_6_h(void) { return res_bit(6, &registers.H); }

// 0xB5: Reset bit 6 of reg-L
int res_6_l(void) { return res_bit(6, &registers.L); }

// 0xB6: Reset bit 6 of memory(HL)
int res_6_hl(void) { return res_bit_hl(6); }

// 0xB7: Reset bit 6 of reg-A
int res_6_a(void) { return res_bit(6, &registers.A); }

// 0xB8: Reset bit 7 of reg-B
int res_7_b(void) { return res_bit(7, &registers.B); }

// 0xB9: Reset bit 7 of reg-C
int res_7_c(void) { return res_bit(7, &registers.C); }

// 0xBA: Reset bit 7 of reg-D
int res_7_d(void) { return res_bit(7, &registers.D); }

// 0xBB: Reset bit 7 of reg-E
int res_7_e(void) { return res_bit(7, &registers.E); }

// 0xBC: Reset bit 7 of reg-H
int res_7_h(void) { return res_bit(7, &registers.H); }

// 0xBD: Reset bit 7 of reg-L
int res_7_l(void) { return res_bit(7, &registers.L); }

// 0xBE: Reset bit 7 of memory(HL)
int res_7_hl(void) { return res_bit_hl(7); }

// 0xBF: Reset bit 7 of reg-A
int res_7_a(void) { return res_bit(7, &registers.A); }

// 0xC0: Set bit 0 of reg-B
int set_0_b(void) { return set_bit(0, &registers.B); }

// 0xC1: Set bit 0 of reg-C
int set_0_c(void) { return set_bit(0, &registers.C); }

// 0xC2: Set bit 0 of reg-D
int set_0_d(void) { return set_bit(0, &registers.D); }

// 0xC3: Set bit 0 of reg-E
int set_0_e(void) { return set_bit(0, &registers.E); }

// 0xC4: Set bit 0 of reg-H
int set_0_h(void) { return set_bit(0, &registers.H); }

// 0xC5: Set bit 0 of reg-L
int set_0_l(void) { return set_bit(0, &registers.L); }

// 0xC6: Set bit 0 of memory(HL)
int set_0_hl(void) { return set_bit_hl(0); }

// 0xC7: Set bit 0 of reg-A
int set_0_a(void) { return set_bit(0, &registers.A); }

// 0xC8: Set bit 1 of reg-B
int set_1_b(void) { return set_bit(1, &registers.B); }

// 0xC9: Set bit 1 of reg-C
int set_1_c(void) { return set_bit(1, &registers.C); }

// 0xCA: Set bit 1 of reg-D
int set_1_d(void) { return set_bit(1, &registers.D); }

// 0xCB: Set bit 1 of reg-E
int set_1_e(void) { return set_bit(1, &registers.E); }

// 0xCC: Set bit 1 of reg-H
int set_1_h(void) { return set_bit(1, &registers.H); }

// 0xCD: Set bit 1 of reg-L
int set_1_l(void) { return set_bit(1, &registers.L); }

// 0xCE: Set bit 1 of memory(HL)
int set_1_hl(void) { return set_bit_hl(1); }

// 0xCF: Set bit 1 of reg-A
int set_1_a(void) { return set_bit(1, &registers.A); }

// 0xD0: Set bit 2 of reg-B
int set_2_b(void) { return set_bit(2, &registers.B); }

// 0xD1: Set bit 2 of reg-C
int set_2_c(void) { return set_bit(2, &registers.C); }

// 0xD2: Set bit 2 of reg-D
int set_2_d(void) { return set_bit(2, &registers.D); }

// 0xD3: Set bit 2 of reg-E
int set_2_e(void) { return set_bit(2, &registers.E); }

// 0xD4: Set bit 2 of reg-H
int set_2_h(void) { return set_bit(2, &registers.H); }

// 0xD5: Set bit 2 of reg-L
int set_2_l(void) { return set_bit(2, &registers.L); }

// 0xD6: Set bit 2 of memory(HL)
int set_2_hl(void) { return set_bit_hl(2); }

// 0xD7: Set bit 2 of reg-A
int set_2_a(void) { return set_bit(2, &registers.A); }

// 0xD8: Set bit 3 of reg-B
int set_3_b(void) { return set_bit(3, &registers.B); }

// 0xD9: Set bit 3 of reg-C
int set_3_c(void) { return set_bit(3, &registers.C); }

// 0xDA: Set bit 3 of reg-D
int set_3_d(void) { return set_bit(3, &registers.D); }

// 0xDB: Set bit 3 of reg-E
int set_3_e(void) { return set_bit(3, &registers.E); }

// 0xDC: Set bit 3 of reg-H
int set_3_h(void) { return set_bit(3, &registers.H); }

// 0xDD: Set bit 3 of reg-L
int set_3_l(void) { return set_bit(3, &registers.L); }

// 0xDE: Set bit 3 of memory(HL)
int set_3_hl(void) { return set_bit_hl(3); }

// 0xDF: Set bit 3 of reg-A
int set_3_a(void) { return set_bit(3, &registers.A); }

// 0xE0: Set bit 4 of reg-B
int set_4_b(void) { return set_bit(4, &registers.B); }

// 0xE1: Set bit 4 of reg-C
int set_4_c(void) { return set_bit(4, &registers.C); }

// 0xE2: Set bit 4 of reg-D
int set_4_d(void) { return set_bit(4, &registers.D); }

// 0xE3: Set bit 4 of reg-E
int set_4_e(void) { return set_bit(4, &registers.E); }

// 0xE4: Set bit 4 of reg-H
int set_4_h(void) { return set_bit(4, &registers.H); }

// 0xE5: Set bit 4 of reg-L
int set_4_l(void) { return set_bit(4, &registers.L); }

// 0xE6: Set bit 4 of memory(HL)
int set_4_hl(void) { return set_bit_hl(4); }

// 0xE7: Set bit 4 of reg-A
int set_4_a(void) { return set_bit(4, &registers.A); }

// 0xE8: Set bit 5 of reg-B
int set_5_b(void) { return set_bit(5, &registers.B); }

// 0xE9: Set bit 5 of reg-C
int set_5_c(void) { return set_bit(5, &registers.C); }

// 0xEA: Set bit 5 of reg-D
int set_5_d(void) { return set_bit(5, &registers.D); }

// 0xEB: Set bit 5 of reg-E
int set_5_e(void) { return set_bit(5, &registers.E); }

// 0xEC: Set bit 5 of reg-H
int set_5_h(void) { return set_bit(5, &registers.H); }

// 0xED: Set bit 5 of reg-L
int set_5_l(void) { return set_bit(5, &registers.L); }

// 0xEE: Set bit 5 of memory(HL)
int set_5_hl(void) { return set_bit_hl(5); }

// 0xEF: Set bit 5 of reg-A
int set_5_a(void) { return set_bit(5, &registers.A); }

// 0xF0: Set bit 6 of reg-B
int set_6_b(void) { return set_bit(6, &registers.B); }

// 0xF1: Set bit 6 of reg-C
int set_6_c(void) { return set_bit(6, &registers.C); }

// 0xF2: Set bit 6 of reg-D
int set_6_d(void) { return set_bit(6, &registers.D); }

// 0xF3: Set bit 6 of reg-E
int set_6_e(void) { return set_bit(6, &registers.E); }

// 0xF4: Set bit 6 of reg-H
int set_6_h(void) { return set_bit(6, &registers.H); }

// 0xF5: Set bit 6 of reg-L
int set_6_l(void) { return set_bit(6, &registers.L); }

// 0xF6: Set bit 6 of memory(HL)
int set_6_hl(void) { return set_bit_hl(6); }

// 0xF7: Set bit 6 of reg-A
int set_6_a(void) { return set_bit(6, &registers.A); }

// 0xF8: Set bit 7 of reg-B
int set_7_b(void) { return set_bit(7, &registers.B); }

// 0xF9: Set bit 7 of reg-C
int set_7_c(void) { return set_bit(7, &registers.C); }

// 0xFA: Set bit 7 of reg-D
int set_7_d(void) { return set_bit(7, &registers.D); }

// 0xFB: Set bit 7 of reg-E
int set_7_e(void) { return set_bit(7, &registers.E); }

// 0xFC: Set bit 7 of reg-H
int set_7_h(void) { return set_bit(7, &registers.H); }

// 0xFD: Set bit 7 of reg-L
int set_7_l(void) { return set_bit(7, &registers.L); }

// 0xFE: Set bit 7 of memory(HL)
int set_7_hl(void) { return set_bit_hl(7); }

// 0xFF: Set bit 7 of reg-A
int set_7_a(void) { return set_bit(7, &registers.A); }