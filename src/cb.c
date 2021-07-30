#include <stdlib.h>
#include "cb.h"
#include "cb_instructions.h"
#include "registers.h"
#include "memory.h"
#include "cpu.h"

int (*cb_instructions[256])(void) = {
/*0x0*/	rlc_b, rlc_c, rlc_d, rlc_e, rlc_h, rlc_l, rlc_hl, rlc_a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x1*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x2*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x3*/	swap_b, swap_c, swap_d, swap_e, swap_h, swap_l, swap_hl, swap_a, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
/*0x4*/	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
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

//region Helpers

//region 8-bit SWAP

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
int rlc(unsigned char *reg, int cycles)
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

	return cycles;
}



//endregion

//region CB Instructions

// 0x00: Rotate reg-B left (+ new carry flag), set carry flag with MSB
int rlc_b(void) { return rlc(&registers.B, 8); }

// 0x00: Rotate reg-C left (+ new carry flag), set carry flag with MSB
int rlc_c(void) { return rlc(&registers.C, 8); }

// 0x00: Rotate reg-D left (+ new carry flag), set carry flag with MSB
int rlc_d(void) { return rlc(&registers.D, 8); }

// 0x00: Rotate reg-E left (+ new carry flag), set carry flag with MSB
int rlc_e(void) { return rlc(&registers.E, 8); }

// 0x00: Rotate reg-H left (+ new carry flag), set carry flag with MSB
int rlc_h(void) { return rlc(&registers.H, 8); }

// 0x00: Rotate reg-L left (+ new carry flag), set carry flag with MSB
int rlc_l(void) { return rlc(&registers.L, 8); }

// 0x00: Rotate memory(HL) left (+ new carry flag), set carry flag with MSB
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
int rlc_a(void) { return rlc(&registers.A, 8); }

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

//endregion




