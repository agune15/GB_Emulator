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

TEST_CASE("0x06: 8-bit load from memory to reg-B", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x06;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.B == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x0E: 8-bit load from memory to reg-C", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x0E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.C == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x16: 8-bit load from memory to reg-D", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x16;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.D == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x1E: 8-bit load from memory to reg-E", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x1E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.E == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x26: 8-bit load from memory to reg-H", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x26;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.H == value);
	CHECK(cycles == 8);
}

TEST_CASE("0x2E: 8-bit load from memory to reg-L", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x2E;
	int value = ROM_banks[registers.PC+1] = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.L == value);
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


