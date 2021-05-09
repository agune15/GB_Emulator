#define	 CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/cpu.c"
};

// Test helpers
void add_a_test(unsigned char valueToAdd, int opCycles);
void adc_a_test(unsigned char valueToAdd, int opCycles);
void sub_a_test(unsigned char valueToSub, int opCycles);
void sbc_a_test(unsigned char valueToSub, int opCycles);
void and_a_test(unsigned char valueToAnd, int opCycles);
void or_a_test(unsigned char valueToOr, int opCycles);
void xor_a_test(unsigned char valueToXor, int opCycles);

//region Others

TEST_CASE("Registers initialization", "[CPU tests]") {
	init_registers();

	CHECK(registers.AF == 0x01B0);
	CHECK(registers.BC == 0x0013);
	CHECK(registers.DE == 0x00D8);
	CHECK(registers.HL == 0x014D);
	CHECK(registers.SP == 0xFFFE);
	CHECK(registers.PC == 0x100);
}

//endregion

//region Instructions

TEST_CASE("0x01: Load from memory(nn) to reg-BC", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x01;
	unsigned short value = GENERATE(take(10, random(0x0000, 0xFFFF)));
	ROM_banks[registers.PC+1] = value & 0x00FF;
	ROM_banks[registers.PC+2] = (value & 0xFF00) >> 8;

	int cycles = execute_next_instruction();
	CHECK(registers.BC == value);
	CHECK(cycles == 12);
}

TEST_CASE("0x02: Load from reg-A to memory(BC)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x02;
	registers.BC = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.A = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.BC) == registers.A);
	CHECK(cycles == 8);
}

TEST_CASE("0x06: Load from memory(n) to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x06;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == value);
	CHECK(cycles == 8);
}


TEST_CASE("0x08: Load from reg-SP to memory address pointed in(nn)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x08;
	unsigned short address = GENERATE(take(10, random(0x8000, 0xFE9F)));
	ROM_banks[registers.PC+1] = address & 0x00FF;
	ROM_banks[registers.PC+2] = (address & 0xFF00) >> 8;
	registers.SP = GENERATE(take(5, random(0, 0xFFFF)));

	int cycles = execute_next_instruction();
	CHECK(read_short(address) == registers.SP);
	CHECK(cycles == 20);
}

TEST_CASE("0x0A: Load from memory(BC) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0A;
	registers.BC = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.BC, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(registers.BC));
	CHECK(cycles == 8);
}

TEST_CASE("0x0E: Load from memory(n) to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x11: Load from memory(nn) to reg-DE", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x11;
	unsigned short value = GENERATE(take(10, random(0x0000, 0xFFFF)));
	ROM_banks[registers.PC+1] = value & 0x00FF;
	ROM_banks[registers.PC+2] = (value & 0xFF00) >> 8;

	int cycles = execute_next_instruction();
	CHECK(registers.DE == value);
	CHECK(cycles == 12);
}

TEST_CASE("0x12: Load from reg-A to memory(DE)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x12;
	registers.DE = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.A = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.DE) == registers.A);
	CHECK(cycles == 8);
}

TEST_CASE("0x16: Load from memory(n) to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x16;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x1A: Load from memory(DE) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1A;
	registers.DE = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.DE, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(registers.DE));
	CHECK(cycles == 8);
}

TEST_CASE("0x1E: Load from memory(n) to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x21: Load from memory(nn) to reg-DE", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x21;
	unsigned short value = GENERATE(take(10, random(0x0000, 0xFFFF)));
	ROM_banks[registers.PC+1] = value & 0x00FF;
	ROM_banks[registers.PC+2] = (value & 0xFF00) >> 8;

	int cycles = execute_next_instruction();
	CHECK(registers.HL == value);
	CHECK(cycles == 12);
}

TEST_CASE("0x22: Load from reg-A to memory(HL), increment reg-HL", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x22;
	int prev_HL = registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.A = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(prev_HL) == registers.A);
	CHECK(registers.HL == prev_HL + 1);
	CHECK(cycles == 8);
}

TEST_CASE("0x26: Load from memory(n) to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x26;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x2A: Load from memory(HL) to reg-A, increment reg-HL", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2A;
	int prev_HL = registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(prev_HL));
	CHECK(registers.HL == prev_HL + 1);
	CHECK(cycles == 8);
}

TEST_CASE("0x2E: Load from memory(n) to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x31: Load from memory(nn) to reg-SP", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x31;
	unsigned short value = GENERATE(take(10, random(0x0000, 0xFFFF)));
	ROM_banks[registers.PC+1] = value & 0x00FF;
	ROM_banks[registers.PC+2] = (value & 0xFF00) >> 8;

	int cycles = execute_next_instruction();
	CHECK(registers.SP == value);
	CHECK(cycles == 12);
}

TEST_CASE("0x32: Load from reg-A to memory(HL), decrement reg-HL", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x32;
	int prev_HL = registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.A = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(prev_HL) == registers.A);
	CHECK(registers.HL == prev_HL - 1);
	CHECK(cycles == 8);
}

TEST_CASE("0x36: Load from memory(n) to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x36;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == value);
	CHECK(cycles == 12);
}

TEST_CASE("0x3A: Load from memory(HL) to reg-A, decrement reg-HL", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3A;
	int prev_HL = registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(prev_HL));
	CHECK(registers.HL == prev_HL - 1);
	CHECK(cycles == 8);
}

TEST_CASE("0x3E: Load from memory(n) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x3E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x40: Load from reg-B to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x40;
	int value = registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x41: Load from reg-C to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x41;
	registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == registers.C);
	CHECK(cycles == 4);
}

TEST_CASE("0x42: Load from reg-D to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x42;
	registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == registers.D);
	CHECK(cycles == 4);
}

TEST_CASE("0x43: Load from reg-E to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x43;
	registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == registers.E);
	CHECK(cycles == 4);
}

TEST_CASE("0x44: Load from reg-H to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x44;
	registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == registers.H);
	CHECK(cycles == 4);
}

TEST_CASE("0x45: Load from reg-L to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x45;
	registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == registers.L);
	CHECK(cycles == 4);
}

TEST_CASE("0x46: Load from memory(HL) to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x46;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.B == read_byte(registers.HL));
	CHECK(cycles == 8);
}

TEST_CASE("0x47: Load from reg-A to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x47;
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == registers.A);
	CHECK(cycles == 4);
}

TEST_CASE("0x48: Load from reg-B to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x48;
	registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == registers.B);
	CHECK(cycles == 4);
}

TEST_CASE("0x49: Load from reg-C to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x49;
	int value = registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x4A: Load from reg-D to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4A;
	registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == registers.D);
	CHECK(cycles == 4);
}

TEST_CASE("0x4B: Load from reg-E to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4B;
	registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == registers.E);
	CHECK(cycles == 4);
}

TEST_CASE("0x4C: Load from reg-H to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4C;
	registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == registers.H);
	CHECK(cycles == 4);
}

TEST_CASE("0x4D: Load from reg-L to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4D;
	registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == registers.L);
	CHECK(cycles == 4);
}

TEST_CASE("0x4E: Load from memory(HL) to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4E;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.C == read_byte(registers.HL));
	CHECK(cycles == 8);
}

TEST_CASE("0x4F: Load from reg-A to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x4F;
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == registers.A);
	CHECK(cycles == 4);
}

TEST_CASE("0x50: Load from reg-B to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x50;
	registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == registers.B);
	CHECK(cycles == 4);
}

TEST_CASE("0x51: Load from reg-C to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x51;
	registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == registers.C);
	CHECK(cycles == 4);
}

TEST_CASE("0x52: Load from reg-D to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x52;
	int value = registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x53: Load from reg-E to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x53;
	registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == registers.E);
	CHECK(cycles == 4);
}

TEST_CASE("0x54: Load from reg-H to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x54;
	registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == registers.H);
	CHECK(cycles == 4);
}

TEST_CASE("0x55: Load from reg-L to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x55;
	registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == registers.L);
	CHECK(cycles == 4);
}

TEST_CASE("0x56: Load from memory(HL) to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x56;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.D == read_byte(registers.HL));
	CHECK(cycles == 8);
}

TEST_CASE("0x57: Load from reg-A to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x57;
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == registers.A);
	CHECK(cycles == 4);
}

TEST_CASE("0x58: Load from reg-B to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x58;
	registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == registers.B);
	CHECK(cycles == 4);
}

TEST_CASE("0x59: Load from reg-C to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x59;
	registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == registers.C);
	CHECK(cycles == 4);
}

TEST_CASE("0x5A: Load from reg-D to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5A;
	int value = registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x5B: Load from reg-E to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5B;
	int value = registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x5C: Load from reg-H to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5C;
	registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == registers.H);
	CHECK(cycles == 4);
}

TEST_CASE("0x5D: Load from reg-L to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5D;
	registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == registers.L);
	CHECK(cycles == 4);
}

TEST_CASE("0x5E: Load from memory(HL) to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5E;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.E == read_byte(registers.HL));
	CHECK(cycles == 8);
}

TEST_CASE("0x5F: Load from reg-A to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x5F;
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == registers.A);
	CHECK(cycles == 4);
}

TEST_CASE("0x60: Load from reg-B to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x60;
	registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == registers.B);
	CHECK(cycles == 4);
}

TEST_CASE("0x61: Load from reg-C to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x61;
	registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == registers.C);
	CHECK(cycles == 4);
}

TEST_CASE("0x62: Load from reg-D to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x62;
	registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == registers.D);
	CHECK(cycles == 4);
}

TEST_CASE("0x63: Load from reg-E to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x63;
	registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == registers.E);
	CHECK(cycles == 4);
}

TEST_CASE("0x64: Load from reg-H to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x64;
	int value = registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x65: Load from reg-L to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x65;
	registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == registers.L);
	CHECK(cycles == 4);
}

TEST_CASE("0x66: Load from memory(HL) to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x66;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.H == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x67: Load from reg-A to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x67;
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == registers.A);
	CHECK(cycles == 4);
}

TEST_CASE("0x68: Load from reg-B to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x68;
	registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == registers.B);
	CHECK(cycles == 4);
}

TEST_CASE("0x69: Load from reg-C to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x69;
	registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == registers.C);
	CHECK(cycles == 4);
}

TEST_CASE("0x6A: Load from reg-D to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6A;
	int value = registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x6B: Load from reg-E to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6B;
	registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == registers.E);
	CHECK(cycles == 4);
}

TEST_CASE("0x6C: Load from reg-H to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6C;
	registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == registers.H);
	CHECK(cycles == 4);
}

TEST_CASE("0x6D: Load from reg-L to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6D;
	int value = registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x6E: Load from memory(HL) to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6E;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.L == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x6F: Load from reg-A to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x6F;
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == registers.A);
	CHECK(cycles == 4);
}

TEST_CASE("0x70: Load from reg-B to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x70;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.B = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.B);
	CHECK(cycles == 8);
}

TEST_CASE("0x71: Load from reg-C to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x71;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.C = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.C);
	CHECK(cycles == 8);
}

TEST_CASE("0x72: Load from reg-D to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x72;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.D = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.D);
	CHECK(cycles == 8);
}

TEST_CASE("0x73: Load from reg-E to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x73;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.E = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.E);
	CHECK(cycles == 8);
}

TEST_CASE("0x74: Load from reg-H to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x74;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.H);
	CHECK(cycles == 8);
}

TEST_CASE("0x75: Load from reg-L to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x75;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.L);
	CHECK(cycles == 8);
}

TEST_CASE("0x77: Load from reg-A to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x77;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	registers.A = GENERATE(take(1, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == registers.A);
	CHECK(cycles == 8);
}

TEST_CASE("0x78: Load from reg-B to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x78;
	registers.B = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == registers.B);
	CHECK(cycles == 4);
}

TEST_CASE("0x79: Load from reg-C to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x79;
	registers.C = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == registers.C);
	CHECK(cycles == 4);
}

TEST_CASE("0x7A: Load from reg-D to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7A;
	registers.D = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == registers.D);
	CHECK(cycles == 4);
}

TEST_CASE("0x7B: Load from reg-E to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7B;
	registers.E = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == registers.E);
	CHECK(cycles == 4);
}

TEST_CASE("0x7C: Load from reg-H to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7C;
	registers.H = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == registers.H);
	CHECK(cycles == 4);
}

TEST_CASE("0x7D: Load from reg-L to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7D;
	registers.L = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == registers.L);
	CHECK(cycles == 4);
}

TEST_CASE("0x7E: Load from memory(HL) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7E;
	registers.HL = GENERATE(take(10, random(0x8000, 0xFE9F)));
	int value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(registers.HL));
	CHECK(cycles == 8);
}

TEST_CASE("0x7F: Load from reg-A to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7F;
	int value = registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == value);
	CHECK(cycles == 4);
}

TEST_CASE("0x80: Add reg-B to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x80;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.B, 4);
}

TEST_CASE("0x81: Add reg-C to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x81;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.C, 4);
}

TEST_CASE("0x82: Add reg-D to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x82;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.D, 4);
}

TEST_CASE("0x83: Add reg-E to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x83;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.E, 4);
}

TEST_CASE("0x84: Add reg-H to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x84;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.H, 4);
}

TEST_CASE("0x85: Add reg-L to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x85;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.L, 4);
}

TEST_CASE("0x86: Add memory(HL) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x86;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	add_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0x87: Add reg-A to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x87;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(registers.A, 4);
}

TEST_CASE("0x88: Add reg-B (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x88;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.B, 4);
}

TEST_CASE("0x89: Add reg-C (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x89;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.C, 4);
}

TEST_CASE("0x8A: Add reg-D (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x8A;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.D, 4);
}

TEST_CASE("0x8B: Add reg-E (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x8B;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.E, 4);
}

TEST_CASE("0x8C: Add reg-H (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x8C;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.H, 4);
}

TEST_CASE("0x8D: Add reg-L (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x8D;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.L, 4);
}

TEST_CASE("0x8E: Add memory(HL) (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x8E;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	adc_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0x8F: Add reg-A (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x8F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(registers.A, 4);
}

TEST_CASE("0x90: Subtract reg-B from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x90;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.B, 4);
}

TEST_CASE("0x91: Subtract reg-C from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x91;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.C, 4);
}

TEST_CASE("0x92: Subtract reg-D from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x92;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.D, 4);
}

TEST_CASE("0x93: Subtract reg-E from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x93;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.E, 4);
}

TEST_CASE("0x94: Subtract reg-H from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x94;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.H, 4);
}

TEST_CASE("0x95: Subtract reg-L from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x95;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.L, 4);
}

TEST_CASE("0x96: Subtract memory(HL) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x96;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	sub_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0x97: Subtract reg-A from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x97;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(registers.A, 4);
}

TEST_CASE("0x98: Subtract reg-B (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x98;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.B, 4);
}

TEST_CASE("0x99: Subtract reg-C (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x99;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.C, 4);
}

TEST_CASE("0x9A: Subtract reg-D (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x9A;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.D, 4);
}

TEST_CASE("0x9B: Subtract reg-E (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x9B;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.E, 4);
}

TEST_CASE("0x9C: Subtract reg-H (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x9C;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.H, 4);
}

TEST_CASE("0x9D: Subtract reg-L (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x9D;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.L, 4);
}

TEST_CASE("0x9E: Subtract memory(HL) (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x9E;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	sbc_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0x9F: Subtract reg-A (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x9F;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(registers.A, 4);
}

TEST_CASE("0xA0: Logical AND, reg-B & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA0;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.B, 4);
}

TEST_CASE("0xA1: Logical AND, reg-C & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA1;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.C, 4);
}

TEST_CASE("0xA2: Logical AND, reg-D & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA2;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.D, 4);
}

TEST_CASE("0xA3: Logical AND, reg-E & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA3;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.E, 4);
}

TEST_CASE("0xA4: Logical AND, reg-H & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA4;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.H, 4);
}

TEST_CASE("0xA5: Logical AND, reg-L & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA5;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.L, 4);
}

TEST_CASE("0xA6: Logical AND, memory(HL) & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA6;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	and_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0xA7: Logical AND, reg-A & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA7;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(registers.A, 4);
}

TEST_CASE("0xA8: Logical XOR, reg-B ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA8;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.B, 4);
}

TEST_CASE("0xA9: Logical XOR, reg-C ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xA9;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.C, 4);
}

TEST_CASE("0xAA: Logical XOR, reg-D ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xAA;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.D, 4);
}

TEST_CASE("0xAB: Logical XOR, reg-E ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xAB;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.E, 4);
}

TEST_CASE("0xAC: Logical XOR, reg-H ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xAC;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.H, 4);
}

TEST_CASE("0xAD: Logical XOR, reg-L ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xAD;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.L, 4);
}

TEST_CASE("0xAE: Logical XOR, memory(HL) ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xAE;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	xor_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0xAF: Logical XOR, reg-A ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xAF;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(registers.A, 4);
}

TEST_CASE("0xB0: Logical OR, reg-B | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB0;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.B = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.B, 4);
}

TEST_CASE("0xB1: Logical OR, reg-C | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB1;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.C = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.C, 4);
}

TEST_CASE("0xB2: Logical OR, reg-D | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB2;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.D = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.D, 4);
}

TEST_CASE("0xB3: Logical OR, reg-E | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB3;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.E = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.E, 4);
}

TEST_CASE("0xB4: Logical OR, reg-H | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB4;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.H = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.H, 4);
}

TEST_CASE("0xB5: Logical OR, reg-L | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB5;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.L = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.L, 4);
}

TEST_CASE("0xB6: Logical OR, memory(HL) | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB6;
	registers.A = GENERATE(take(5, random(0, 0xFF)));
	registers.HL = GENERATE(take(5, random(0x8000, 0xFE9F)));
	unsigned char value = GENERATE(take(1, random(0, 0xFF)));
	write_byte(registers.HL, value);

	or_a_test(read_byte(registers.HL), 8);
}

TEST_CASE("0xB7: Logical OR, reg-L | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xB7;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(registers.A, 4);
}

TEST_CASE("0xC1: Pop from stack to reg-BC, increment SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xC1;
	registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	unsigned short word = GENERATE(take(1, random(0, 0xFFFF)));
	push_short_stack(word);

	int cycles = execute_next_instruction();
	CHECK(registers.BC == word);
	CHECK(cycles == 12);
}

TEST_CASE("0xC5: Push reg-BC to stack, decrement SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xC5;
	unsigned short address = registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	registers.BC = GENERATE(take(1, random(0, 0xFFFF)));

	int cycles = execute_next_instruction();
	CHECK(read_short(address-2) == registers.BC);
	CHECK(registers.SP == address-2);
	CHECK(cycles == 16);
}

TEST_CASE("0xC6: Add memory(n) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xC6;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	add_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xCE: Add memory(n) (+ carry flag) to reg-A", "[cpu][add]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xCE;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	adc_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xD1: Pop from stack to reg-DE, increment SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xD1;
	registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	unsigned short word = GENERATE(take(1, random(0, 0xFFFF)));
	push_short_stack(word);

	int cycles = execute_next_instruction();
	CHECK(registers.DE == word);
	CHECK(cycles == 12);
}

TEST_CASE("0xD5: Push reg-DE to stack, decrement SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xD5;
	unsigned short address = registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	registers.DE = GENERATE(take(1, random(0, 0xFFFF)));

	int cycles = execute_next_instruction();
	CHECK(read_short(address-2) == registers.DE);
	CHECK(registers.SP == address-2);
	CHECK(cycles == 16);
}

TEST_CASE("0xD6: Subtract memory(n) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xD6;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	sub_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xDE: Subtract memory(n) (+ carry flag) from reg-A", "[cpu][sub]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xDE;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	sbc_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xE0: Load from reg-A to memory(0xFF00 + n)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xE0;
	ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));
	if (ROM_banks[registers.PC+1] == 0x00 || ROM_banks[registers.PC+1] == 0x44 || ROM_banks[registers.PC+1] == 0x46)
		ROM_banks[registers.PC+1]++;
	unsigned short address = 0xFF00 + ROM_banks[registers.PC+1];
	registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(address) == registers.A);
	CHECK(cycles == 12);
}

TEST_CASE("0xE1: Pop from stack to reg-HL, increment SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xE1;
	registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	unsigned short word = GENERATE(take(1, random(0, 0xFFFF)));
	push_short_stack(word);

	int cycles = execute_next_instruction();
	CHECK(registers.HL == word);
	CHECK(cycles == 12);
}

TEST_CASE("0xE2: Load from reg-A to memory(0xFF00 + reg-C)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xE2;
	registers.A = GENERATE(take(10, random(0, 0xFF)));
	registers.C = GENERATE(take(10, random(0, 0xFF)));
	if(registers.C == 0x00 || registers.C == 0x44 || registers.C == 0x46)
		registers.C++;

	int cycles = execute_next_instruction();
	CHECK(read_byte(0xFF00 + registers.C) == registers.A);
	CHECK(cycles == 8);
}

TEST_CASE("0xE5: Push reg-HL to stack, decrement SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xE5;
	unsigned short address = registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	registers.HL = GENERATE(take(1, random(0, 0xFFFF)));

	int cycles = execute_next_instruction();
	CHECK(read_short(address-2) == registers.HL);
	CHECK(registers.SP == address-2);
	CHECK(cycles == 16);
}

TEST_CASE("0xE6: Logical AND, memory(n) & reg-A, result in reg-A", "[cpu][and]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xE6;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	and_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xEA: Load from reg-A to memory address pointed in(nn)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xEA;
	unsigned short address = GENERATE(take(10, random(0x8000, 0xFE9F)));
	ROM_banks[registers.PC+1] = address & 0x00FF;
	ROM_banks[registers.PC+2] = (address & 0xFF00) >> 8;
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(address) == registers.A);
	CHECK(cycles == 16);
}

TEST_CASE("0xEE: Logical XOR, memory(n) ^ reg-A, result in reg-A", "[cpu][xor]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xEE;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	xor_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xF0: Load from memory(0xFF00 + n) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF0;
	ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));
	if (ROM_banks[registers.PC+1] == 0x00 || ROM_banks[registers.PC+1] == 0x44 || ROM_banks[registers.PC+1] == 0x46)
		ROM_banks[registers.PC+1]++;
	unsigned short address = 0xFF00 + ROM_banks[registers.PC+1];
	unsigned char value = GENERATE(take(10, random(0, 0xFF)));
	write_byte(address, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(address));
	CHECK(cycles == 12);
}

TEST_CASE("0xF1: Pop from stack to reg-AF, increment SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF1;
	registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	unsigned short word = GENERATE(take(1, random(0, 0xFFFF)));
	push_short_stack(word);

	int cycles = execute_next_instruction();
	CHECK(registers.AF == word);
	CHECK(cycles == 12);
}

TEST_CASE("0xF2: Load from memory(0xFF00 + reg-C) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF2;
	registers.C = GENERATE(take(10, random(0, 0xFF)));
	if(registers.C == 0x00 || registers.C == 0x44 || registers.C == 0x46)
		registers.C++;
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(0xFF00 + registers.C, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == read_byte(0xFF00 + registers.C));
	CHECK(cycles == 8);
}

TEST_CASE("0xF5: Push reg-AF to stack, decrement SP twice", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF5;
	unsigned short address = registers.SP = GENERATE(take(5, random(0xFF82, 0xFFFF)));
	registers.AF = GENERATE(take(1, random(0, 0xFFFF)));

	int cycles = execute_next_instruction();
	CHECK(read_short(address-2) == registers.AF);
	CHECK(registers.SP == address-2);
	CHECK(cycles == 16);
}

TEST_CASE("0xF6: Logical OR, memory(n) | reg-A, result in reg-A", "[cpu][or]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF6;
	ROM_banks[registers.PC+1] = GENERATE(take(5, random(0, 0xFF)));
	registers.A = GENERATE(take(5, random(0, 0xFF)));

	or_a_test(read_byte(registers.PC+1), 8);
}

TEST_CASE("0xF8: Load from reg-SP + (signed)memory(n) to reg-HL", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF8;
	int operand = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));
	registers.SP = GENERATE(take(10, random(0, 0xFFFF)));

	int addition = registers.SP + (signed char)operand;
	int low_nibble_add = (registers.SP & 0x0F) + ((signed char)operand & 0x0F);

	bool carry_flag_state = addition > 0xFFFF;
	bool halfcarry_flag_state = low_nibble_add > 0x0F;

	int cycles = execute_next_instruction();
	CHECK(registers.HL == (unsigned short)addition);
	CHECK(cycles == 12);

	CHECK(is_flag_set(CARRY) == carry_flag_state);
	CHECK(is_flag_set(HALFCARRY) == halfcarry_flag_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(ZERO) == false);
}

TEST_CASE("0xF9: Load from reg-HL to reg-SP", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xF9;
	registers.HL = GENERATE(take(10, random(0, 0xFFFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.SP == registers.HL);
	CHECK(cycles == 8);
}

TEST_CASE("0xFA: Load from memory address pointed in(nn) to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0xFA;
	unsigned short address = GENERATE(take(10, random(0x8000, 0xFE9F)));
	ROM_banks[registers.PC+1] = address & 0x00FF;
	ROM_banks[registers.PC+2] = (address & 0xFF00) >> 8;
	unsigned char value = GENERATE(take(5, random(0, 0xFF)));
	write_byte(address, value);

	int cycles = execute_next_instruction();
	CHECK(registers.A == value);
	CHECK(cycles == 16);
}

//endregion

//region Helpers

void add_a_test(unsigned char valueToAdd, int opCycles)
{
	unsigned int addition = registers.A + valueToAdd;
	int low_nibble_add = (registers.A & 0x0F) + (valueToAdd & 0x0F);

	bool carry_flag_state = addition > 0xFF;
	bool halfcarry_flag_state = low_nibble_add > 0x0F;

	addition = addition & 0xFF;
	bool zero_flag_state = addition == 0;

	int cycles = execute_next_instruction();
	CHECK(registers.A == addition);
	CHECK(cycles == opCycles);

	CHECK(is_flag_set(CARRY) == carry_flag_state);
	CHECK(is_flag_set(HALFCARRY) == halfcarry_flag_state);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(ZERO) == zero_flag_state);
}

void adc_a_test(unsigned char valueToAdd, int opCycles)
{
	if (valueToAdd % 2 == 0)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	valueToAdd += (is_flag_set(CARRY)) ? 1 : 0;

	add_a_test(valueToAdd, opCycles);
}

void sub_a_test(unsigned char valueToSub, int opCycles)
{
	bool carry_flag_state = valueToSub > registers.A;
	bool halfcarry_flag_state = (valueToSub & 0x0F) > (registers.A & 0x0F);

	unsigned char subtract = registers.A - valueToSub;
	bool zero_flag_state = subtract == 0;

	int cycles = execute_next_instruction();
	CHECK(registers.A == subtract);
	CHECK(cycles == opCycles);

	CHECK(is_flag_set(CARRY) == carry_flag_state);
	CHECK(is_flag_set(HALFCARRY) == halfcarry_flag_state);
	CHECK(is_flag_set(NEGATIVE) == true);
	CHECK(is_flag_set(ZERO) == zero_flag_state);
}

void sbc_a_test(unsigned char valueToSub, int opCycles)
{
	if (valueToSub % 2 == 0)
		set_flag(CARRY);
	else
		clear_flag(CARRY);

	valueToSub += (is_flag_set(CARRY)) ? 1 : 0;

	sub_a_test(valueToSub, opCycles);
}

void and_a_test(unsigned char valueToAnd, int opCycles)
{
	unsigned char logical_and = registers.A & valueToAnd;
	bool zero_flag_state = logical_and == 0;

	int cycles = execute_next_instruction();
	CHECK(registers.A == logical_and);
	CHECK(cycles == opCycles);

	CHECK(is_flag_set(CARRY) == false);
	CHECK(is_flag_set(HALFCARRY) == true);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(ZERO) == zero_flag_state);
}

void or_a_test(unsigned char valueToOr, int opCycles)
{
	unsigned char logical_or = registers.A | valueToOr;
	bool zero_flag_state = logical_or == 0;

	int cycles = execute_next_instruction();
	CHECK(registers.A == logical_or);
	CHECK(cycles == opCycles);

	CHECK(is_flag_set(CARRY) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(ZERO) == zero_flag_state);
}

void xor_a_test(unsigned char valueToXor, int opCycles)
{
	unsigned char logical_xor = registers.A ^ valueToXor;
	bool zero_flag_state = logical_xor == 0;

	int cycles = execute_next_instruction();
	CHECK(registers.A == logical_xor);
	CHECK(cycles == opCycles);

	CHECK(is_flag_set(CARRY) == false);
	CHECK(is_flag_set(HALFCARRY) == false);
	CHECK(is_flag_set(NEGATIVE) == false);
	CHECK(is_flag_set(ZERO) == zero_flag_state);
}

//endregion

