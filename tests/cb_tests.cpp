#include "catch.hpp"

extern "C" {
	#include "../src/cb.c"
};

// Test helpers
unsigned char swap_test(unsigned char byteToSwap, int opCycles);
void rlc_test(unsigned char *rot_reg);
void rl_test(unsigned char *rot_reg);
void rrc_test(unsigned char *rot_reg);
void rr_test(unsigned char *rot_reg);
void sla_test(unsigned char *rot_reg);
void sra_test(unsigned char *rot_reg);
void srl_test(unsigned char *rot_reg);
void test_bit_test(int bit_n, unsigned char *reg);
void test_bit_hl_test(int bit_n);

//CB Instructions

TEST_CASE("CB 0x00: Rotate reg-B left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x00;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	rlc_test(&registers.B);
}

TEST_CASE("CB 0x01: Rotate reg-C left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x01;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	rlc_test(&registers.C);
}

TEST_CASE("CB 0x02: Rotate reg-D left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x02;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	rlc_test(&registers.D);
}

TEST_CASE("CB 0x03: Rotate reg-E left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x03;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	rlc_test(&registers.E);
}

TEST_CASE("CB 0x04: Rotate reg-H left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x04;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	rlc_test(&registers.H);
}

TEST_CASE("CB 0x05: Rotate reg-L left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x05;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	rlc_test(&registers.L);
}

TEST_CASE("CB 0x06: Rotate memory(HL) left (+ new carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
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

	rlc_test(&registers.A);
}

TEST_CASE("CB 0x08: Rotate reg-B right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x08;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.B);
}

TEST_CASE("CB 0x09: Rotate reg-C right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x09;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.C);
}

TEST_CASE("CB 0x0A: Rotate reg-D right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.D);
}

TEST_CASE("CB 0x0B: Rotate reg-E right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.E);
}

TEST_CASE("CB 0x0C: Rotate reg-H right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.H);
}

TEST_CASE("CB 0x0D: Rotate reg-L right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.L);
}

TEST_CASE("CB 0x0E: Rotate memory(HL) right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = (rot_value << 7) & 0xFF;
	rot_value >>= 1;
	if(carry_state)
		rot_value |= 0x80;
	bool zero_state = rot_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(read_byte(registers.HL) == rot_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 16);
}

TEST_CASE("CB 0x0F: Rotate reg-A right (+ new carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	rrc_test(&registers.A);
}

TEST_CASE("CB 0x10: Rotate reg-B left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x10;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	rl_test(&registers.B);
}

TEST_CASE("CB 0x11: Rotate reg-C left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x11;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	rl_test(&registers.C);
}

TEST_CASE("CB 0x12: Rotate reg-D left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x12;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	rl_test(&registers.D);
}

TEST_CASE("CB 0x13: Rotate reg-E left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x13;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	rl_test(&registers.E);
}

TEST_CASE("CB 0x14: Rotate reg-H left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x14;
	registers.H = GENERATE(take(5, random(0, 0xFF)));
	rl_test(&registers.H);
}

TEST_CASE("CB 0x15: Rotate reg-L left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x15;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	rl_test(&registers.L);
}

TEST_CASE("CB 0x16: Rotate memory(HL) left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x16;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = rot_value >> 7;
	rot_value <<= 1;
	if (is_flag_set(CARRY))
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

TEST_CASE("CB 0x17: Rotate reg-A left (+ old carry flag), set carry flag with MSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x17;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	rl_test(&registers.A);
}

TEST_CASE("CB 0x18: Rotate reg-B right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x18;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	rr_test(&registers.B);
}

TEST_CASE("CB 0x19: Rotate reg-C right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x19;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	rr_test(&registers.C);
}

TEST_CASE("CB 0x1A: Rotate reg-D right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	rr_test(&registers.D);
}

TEST_CASE("CB 0x1B: Rotate reg-E right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	rr_test(&registers.E);
}

TEST_CASE("CB 0x1C: Rotate reg-H right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));
	rr_test(&registers.H);
}

TEST_CASE("CB 0x1D: Rotate reg-L right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	rr_test(&registers.L);
}

TEST_CASE("CB 0x1E: Rotate memory(HL) right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = (rot_value << 7) & 0xFF;
	rot_value >>= 1;
	if (is_flag_set(CARRY))
		rot_value |= 0x80;
	bool zero_state = rot_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(read_byte(registers.HL) == rot_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 16);
}

TEST_CASE("CB 0x1F: Rotate reg-A right (+ old carry flag), set carry flag with LSB", "[cpu][cb][rotate]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	rr_test(&registers.A);
}

TEST_CASE("CB 0x20: Shift reg-B left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x20;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.B);
}

TEST_CASE("CB 0x21: Shift reg-C left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x21;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.C);
}

TEST_CASE("CB 0x22: Shift reg-D left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x22;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.D);
}

TEST_CASE("CB 0x23: Shift reg-E left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x23;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.E);
}

TEST_CASE("CB 0x24: Shift reg-H left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x24;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.H);
}

TEST_CASE("CB 0x25: Shift reg-L left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x25;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.L);
}

TEST_CASE("CB 0x26: Shift memory(HL) left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x26;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = rot_value >> 7;
	rot_value <<= 1;
	bool zero_state = rot_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(read_byte(registers.HL) == rot_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 16);
}

TEST_CASE("CB 0x27: Shift reg-A left into carry flag, set LSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x27;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	sla_test(&registers.A);
}

TEST_CASE("CB 0x28: Shift reg-B right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x28;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.B);
}

TEST_CASE("CB 0x29: Shift reg-C right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x29;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.C);
}

TEST_CASE("CB 0x2A: Shift reg-D right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.D);
}

TEST_CASE("CB 0x2B: Shift reg-E right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.E);
}

TEST_CASE("CB 0x2C: Shift reg-H right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.H);
}

TEST_CASE("CB 0x2D: Shift reg-L right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.L);
}

TEST_CASE("CB 0x2E: Shift memory(HL) right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = (rot_value << 7) & 0xFF;
	rot_value = (rot_value & 0x80) | (rot_value >> 1);
	bool zero_state = rot_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(read_byte(registers.HL) == rot_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 16);
}

TEST_CASE("CB 0x2F: Shift reg-A right into carry flag, MSB does not change", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	sra_test(&registers.A);
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

TEST_CASE("CB 0x38: Shift reg-B right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x38;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.B);
}

TEST_CASE("CB 0x39: Shift reg-C right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x39;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.C);
}

TEST_CASE("CB 0x3A: Shift reg-D right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.D);
}

TEST_CASE("CB 0x3B: Shift reg-E right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.E);
}

TEST_CASE("CB 0x3C: Shift reg-H right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.H);
}

TEST_CASE("CB 0x3D: Shift reg-L right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.L);
}

TEST_CASE("CB 0x3E: Shift memory(HL) right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char rot_value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, rot_value);

	bool carry_state = (rot_value << 7) & 0xFF;
	rot_value >>= 1;
	bool zero_state = rot_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(read_byte(registers.HL) == rot_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 16);
}

TEST_CASE("CB 0x3F: Shift reg-A right into carry flag, set MSB to 0", "[cpu][cb][shift]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	srl_test(&registers.A);
}

TEST_CASE("CB 0x40: Test bit 0 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x40;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.B);
}

TEST_CASE("CB 0x41: Test bit 0 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x41;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.C);
}

TEST_CASE("CB 0x42: Test bit 0 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x42;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.D);
}

TEST_CASE("CB 0x43: Test bit 0 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x43;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.E);
}

TEST_CASE("CB 0x44: Test bit 0 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x44;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.H);
}

TEST_CASE("CB 0x45: Test bit 0 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x45;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.L);
}

TEST_CASE("CB 0x46: Test bit 0 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x46;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(0);
}

TEST_CASE("CB 0x47: Test bit 0 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x47;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(0, &registers.A);
}

TEST_CASE("CB 0x48: Test bit 1 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x48;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.B);
}

TEST_CASE("CB 0x49: Test bit 1 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x49;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.C);
}

TEST_CASE("CB 0x4A: Test bit 1 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.D);
}

TEST_CASE("CB 0x4B: Test bit 1 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.E);
}

TEST_CASE("CB 0x4C: Test bit 1 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.H);
}

TEST_CASE("CB 0x4D: Test bit 1 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.L);
}

TEST_CASE("CB 0x4E: Test bit 1 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(1);
}

TEST_CASE("CB 0x4F: Test bit 1 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(1, &registers.A);
}

TEST_CASE("CB 0x50: Test bit 2 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x50;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.B);
}

TEST_CASE("CB 0x51: Test bit 2 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x51;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.C);
}

TEST_CASE("CB 0x52: Test bit 2 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x52;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.D);
}

TEST_CASE("CB 0x53: Test bit 2 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x53;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.E);
}

TEST_CASE("CB 0x54: Test bit 2 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x54;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.H);
}

TEST_CASE("CB 0x55: Test bit 2 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x55;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.L);
}

TEST_CASE("CB 0x56: Test bit 2 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x56;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(2);
}

TEST_CASE("CB 0x57: Test bit 2 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x57;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(2, &registers.A);
}

TEST_CASE("CB 0x58: Test bit 3 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x58;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.B);
}

TEST_CASE("CB 0x59: Test bit 3 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x59;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.C);
}

TEST_CASE("CB 0x5A: Test bit 3 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.D);
}

TEST_CASE("CB 0x5B: Test bit 3 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.E);
}

TEST_CASE("CB 0x5C: Test bit 3 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.H);
}

TEST_CASE("CB 0x5D: Test bit 3 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.L);
}

TEST_CASE("CB 0x5E: Test bit 3 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(3);
}

TEST_CASE("CB 0x5F: Test bit 3 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(3, &registers.A);
}

TEST_CASE("CB 0x60: Test bit 4 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x60;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.B);
}

TEST_CASE("CB 0x61: Test bit 4 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x61;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.C);
}

TEST_CASE("CB 0x62: Test bit 4 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x62;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.D);
}

TEST_CASE("CB 0x63: Test bit 4 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x63;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.E);
}

TEST_CASE("CB 0x64: Test bit 4 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x64;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.H);
}

TEST_CASE("CB 0x65: Test bit 4 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x65;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.L);
}

TEST_CASE("CB 0x66: Test bit 4 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x66;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(4);
}

TEST_CASE("CB 0x67: Test bit 4 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x67;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(4, &registers.A);
}

TEST_CASE("CB 0x68: Test bit 5 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x68;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.B);
}

TEST_CASE("CB 0x69: Test bit 5 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x69;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.C);
}

TEST_CASE("CB 0x6A: Test bit 5 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.D);
}

TEST_CASE("CB 0x6B: Test bit 5 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.E);
}

TEST_CASE("CB 0x6C: Test bit 5 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.H);
}

TEST_CASE("CB 0x6D: Test bit 5 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.L);
}

TEST_CASE("CB 0x6E: Test bit 5 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(5);
}

TEST_CASE("CB 0x6F: Test bit 5 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(5, &registers.A);
}

TEST_CASE("CB 0x70: Test bit 6 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x70;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.B);
}

TEST_CASE("CB 0x71: Test bit 6 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x71;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.C);
}

TEST_CASE("CB 0x72: Test bit 6 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x72;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.D);
}

TEST_CASE("CB 0x73: Test bit 6 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x73;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.E);
}

TEST_CASE("CB 0x74: Test bit 6 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x74;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.H);
}

TEST_CASE("CB 0x75: Test bit 6 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x75;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.L);
}

TEST_CASE("CB 0x76: Test bit 6 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x76;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(6);
}

TEST_CASE("CB 0x77: Test bit 6 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x77;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(6, &registers.A);
}

TEST_CASE("CB 0x78: Test bit 7 of reg-B", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x78;
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.B);
}

TEST_CASE("CB 0x79: Test bit 7 of reg-C", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x79;
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.C);
}

TEST_CASE("CB 0x7A: Test bit 7 of reg-D", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7A;
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.D);
}

TEST_CASE("CB 0x7B: Test bit 7 of reg-E", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7B;
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.E);
}

TEST_CASE("CB 0x7C: Test bit 7 of reg-H", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7C;
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.H);
}

TEST_CASE("CB 0x7D: Test bit 7 of reg-L", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7D;
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.L);
}

TEST_CASE("CB 0x7E: Test bit 7 of memory(HL)", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7E;
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(registers.HL, value);

	test_bit_hl_test(7);
}

TEST_CASE("CB 0x7F: Test bit 7 of reg-A", "[cpu][cb][bit]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	test_bit_test(7, &registers.A);
}

// Helpers

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

void rlc_test(unsigned char *rot_reg)
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

void rl_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = rot_reg_value >> 7;
	rot_reg_value <<= 1;
	if (is_flag_set(CARRY))
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

void rrc_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = (rot_reg_value << 7) & 0xFF;
	rot_reg_value >>= 1;
	rot_reg_value |= *rot_reg << 7;
	bool zero_state = rot_reg_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(*rot_reg == rot_reg_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 8);
}

void rr_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = (rot_reg_value << 7) & 0xFF;
	rot_reg_value >>= 1;
	if (is_flag_set(CARRY))
		rot_reg_value |= 0x80;
	bool zero_state = rot_reg_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(*rot_reg == rot_reg_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 8);
}

void sla_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = rot_reg_value >> 7;
	rot_reg_value <<= 1;
	bool zero_state = rot_reg_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(*rot_reg == rot_reg_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 8);
}

void sra_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = (rot_reg_value << 7) & 0xFF;
	rot_reg_value = (rot_reg_value & 0x80) | (rot_reg_value >> 1);
	bool zero_state = rot_reg_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(*rot_reg == rot_reg_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 8);
}

void srl_test(unsigned char *rot_reg)
{
	unsigned char rot_reg_value = *rot_reg;

	bool carry_state = (rot_reg_value << 7) & 0xFF;
	rot_reg_value >>= 1;
	bool zero_state = rot_reg_value == 0;

	int cycles = execute_cb_instruction();
	CHECK(*rot_reg == rot_reg_value);
	CHECK(is_flag_set(CARRY) == carry_state);
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(cycles == 8);
}

void test_bit_test(int bit_n, unsigned char *reg)
{
	bool zero_state = (*reg >> bit_n & 1) == 0;

	int cycles = execute_cb_instruction();
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == true);
	CHECK(cycles == 8);
}

void test_bit_hl_test(int bit_n)
{
	bool zero_state = (read_byte(registers.HL) >> bit_n & 1) == 0;

	int cycles = execute_cb_instruction();
	CHECK(is_flag_set(ZERO) == zero_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(HALFCARRY) == true);
	CHECK(cycles == 16);
}
