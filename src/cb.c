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
/*0x4*/	bit_0_b, bit_0_c, bit_0_d, bit_0_e, bit_0_h, bit_0_l, bit_0_hl, bit_0_a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x5*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x6*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x7*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x8*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x9*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xA*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xB*/ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xC*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xD*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xE*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0xF*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
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
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);
	clear_flag(CARRY);

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
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);
	clear_flag(CARRY);

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
		clear_flag(CARRY);

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if(*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if (*reg)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	*reg <<= 1;

	if (*reg)
		clear_flag(ZERO);
	else
		clear_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

	return 8;
}

// Shift reg right into carry flag, MSB does not change
int sra(unsigned char *reg)
{
	if (*reg & 0x01)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	*reg = (*reg & 0x80) | (*reg >> 1);

	if (*reg)
		clear_flag(ZERO);
	else
		clear_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

	return 8;
}

// Shift reg right into carry flag, set MSB to 0
int srl(unsigned char *reg)
{
	if (*reg & 0x01)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	*reg >>= 1;

	if (*reg)
		clear_flag(ZERO);
	else
		clear_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

	return 8;
}

//endregion

//region Bit tests

// Test bit of reg
int test_bit(int bit_n, unsigned char *reg)
{
	if (*reg >> bit_n & 1)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	set_flag(HALFCARRY);

	return 8;
}

// Test bit of memory(HL)
int test_bit_hl(int bit_n)
{
	if (read_byte(registers.HL) >> bit_n & 1)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	set_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if (value)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if (value)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if (value)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	if (value)
		clear_flag(ZERO);
	else
		set_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	value <<= 1;

	write_byte(registers.HL, value);

	if (value)
		clear_flag(ZERO);
	else
		clear_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	value = (value & 0x80) | (value >> 1);

	write_byte(registers.HL, value);

	if (value)
		clear_flag(ZERO);
	else
		clear_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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
		clear_flag(CARRY);

	value >>= 1;

	write_byte(registers.HL, value);

	if (value)
		clear_flag(ZERO);
	else
		clear_flag(ZERO);

	clear_flag(NEGATIVE);
	clear_flag(HALFCARRY);

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

// 0x47: Test bit 0 of reg-B
int bit_0_a(void) { return test_bit(0, &registers.A); }