#define	 CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "cpu.h"
	#include "registers.h"
	#include "instructions.h"
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

TEST_CASE("0x7F: Load from reg-A to reg-A", "[cpu][load]") {
	registers.PC = 0x0100;
	ROM_banks[registers.PC] = 0x7F;
	int value = registers.A = GENERATE(take(10, random(0, 0xFF)));

	int cycles = execute_next_instruction();
	CHECK(registers.A == value);
	CHECK(cycles == 4);
}


//endregion


