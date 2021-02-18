#define	 CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "cpu.h"
	#include "registers.h"
	#include "memory.h"
};

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

TEST_CASE("0x06: Load from memory(PC) to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x06;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x0E: Load from memory(PC) to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x16: Load from memory(PC) to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x16;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x1E: Load from memory(PC) to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x26: Load from memory(PC) to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x26;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x2E: Load from memory(PC) to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x36: Load from memory(PC) to memory(HL)", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x36;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(read_byte(registers.HL) == value);
	CHECK(cycles == 12);
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


//endregion


