#include "catch.hpp"

extern "C" {
	#include "../src/cb.c"
};

// Test helpers
unsigned char swap_test(unsigned char byteToSwap, int opCycles);
void rotate_test(unsigned char *rot_reg);

//region CB Instructions

TEST_CASE("CB 0x00: Rotate reg-B left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x00;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.B);
}

TEST_CASE("CB 0x01: Rotate reg-C left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x01;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.C);
}

TEST_CASE("CB 0x02: Rotate reg-D left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x02;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.D);
}

TEST_CASE("CB 0x03: Rotate reg-E left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x03;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.E);
}

TEST_CASE("CB 0x04: Rotate reg-H left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x04;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.H);
}

TEST_CASE("CB 0x05: Rotate reg-L left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x05;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.L);
}

TEST_CASE("CB 0x05: Rotate memory(HL) left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x06;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = rot_value >> 7;
	rot_value <<= 1;
	if (carry_state)
		rot_value += 1;
	bool zero_state = rot_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(read_byte(registers.HL) == rot_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 16);
}

TEST_CASE("CB 0x07: Rotate reg-A left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x07;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	rotate_test(&registers.A);
}

TEST_CASE("CB 0x30: Swap reg-B", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x30;
	registers.B = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.B, 8);
	CHECK(registers.B == swapedByte);
}

TEST_CASE("CB 0x31: Swap reg-C", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x31;
	registers.C = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.C, 8);
	CHECK(registers.C == swapedByte);
}

TEST_CASE("CB 0x32: Swap reg-D", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x32;
	registers.D = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.D, 8);
	CHECK(registers.D == swapedByte);
}

TEST_CASE("CB 0x33: Swap reg-E", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x33;
	registers.E = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.E, 8);
	CHECK(registers.E == swapedByte);
}

TEST_CASE("CB 0x34: Swap reg-H", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x34;
	registers.H = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.H, 8);
	CHECK(registers.H == swapedByte);
}

TEST_CASE("CB 0x35: Swap reg-L", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x35;
	registers.L = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.L, 8);
	CHECK(registers.L == swapedByte);
}

TEST_CASE("CB 0x36: Swap memory(HL)", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x36;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	unsigned char swapedByte = swap_test(value, 10);
	CHECK(read_byte(registers.HL) == swapedByte);
}

TEST_CASE("CB 0x37: Swap reg-A", "[cpu][cb][swap]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x37;
	registers.A = GENERATE(take(5, random(0x00, 0xFF)));

	unsigned char swapedByte = swap_test(registers.A, 8);
	CHECK(registers.A == swapedByte);
}

//region Helpers

unsigned char swap_test(unsigned char byteToSwap, int opCycles)
{
	unsigned char swapped_low_nib = (byteToSwap << 4) & 0xF0;
	byteToSwap = (byteToSwap >> 4) & 0x0F;

	byteToSwap |= swapped_low_nib;

	bool zero_state = byteToSwap == 0;

	int cycles = execute_cb_instruction();
	CHECK(cycles == opCycles);

	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(is_flag_set(CARRY) == false);

	return byteToSwap;
}

void rotate_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = rot_reg_value >> 7;
	rot_reg_value <<= 1;
	if (carry_state)
		rot_reg_value += 1;
	bool zero_state = rot_reg_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(*rot_reg == rot_reg_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 8);
}

//endregion
