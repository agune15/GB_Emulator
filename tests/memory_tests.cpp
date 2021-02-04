#include "catch.hpp"

extern "C" {
	#include "memory.h"
	#include "cpu.h"
}

//region Read from memory

TEST_CASE("Read ROM banks", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFF)));
	unsigned char value = ROM_banks[address] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read VRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0x8000, 0x9FFF)));
	unsigned char value = VRAM[address - 0x8000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read exRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0xA000, 0xBFFF)));
	unsigned char value = exRAM[address - 0xA000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read WRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0xC000, 0xDFFF)));
	unsigned char value = WRAM[address - 0xC000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read OAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0xFE00, 0xFE9F)));
	unsigned char value = OAM[address - 0xFE00] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read IO", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0xFF00, 0xFF7F)));
	unsigned char value = IO[address - 0xFF00] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read HRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(100, random(0xFF80, 0xFFFE)));
	unsigned char value = HRAM[address - 0xFF80] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read Interrupt Enable register", "[memory][read][byte]") {
	unsigned short address = 0xFFFF;
	unsigned char value = interrupt_enable_reg = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read short - read-only memory", "[memory][read][short]") {
	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	ROM_banks[address] = word & 0x00FF;
	ROM_banks[address+1] = (word & 0xFF00) >> 8;
	DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(word == read_short(address));
	}
}

TEST_CASE("Read short - memory", "[memory][read][short]") {
	unsigned short address = GENERATE(take(95, random(0x8000, 0xFE9E)),
				   	  take(5, random(0xFF00, 0xFFFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	write_short(address, word);
	DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(word == read_short(address));
	}
}

TEST_CASE("Read short - restricted area", "[memory][read][short]") {
	unsigned short address = GENERATE(take(50, random(0xFEA0, 0xFEFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	write_short(address, word);
	DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(read_short(address) == 0);
	}
}

TEST_CASE("Pull from stack", "[memory][read][stack]") {
	unsigned short address = GENERATE(take(50, random(0xFF82, 0xFFFE)));
	registers.SP = address;
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	push_short_stack(word);
	CHECK(word == pull_short_stack());
	CHECK(registers.SP == address);
}

//endregion

//region Write to memory

TEST_CASE("Write byte - read-only memory", "[memory][write][byte]") {
	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFF)));
	unsigned char value = GENERATE(take(1, random(0x01, 0xFF)));
	ROM_banks[address] = 0x00;
	write_byte(address, value);
	DYNAMIC_SECTION("Write attempt to 0x" << std::hex << address) {
		CHECK(read_byte(address) == 0);
	}
}

TEST_CASE("Write byte - memory", "[memory][write][byte]") {
	unsigned short address = GENERATE(take(95, random(0x8000, 0xFE9F)),
				   	  take(5, random(0xFF00, 0xFFFF)));
	unsigned char value = (address != 0xFF44) ? GENERATE(take(1, random(0x00, 0xFF))) : 0;
	write_byte(address, value);
	DYNAMIC_SECTION("Write to 0x" << std::hex << address) {
		CHECK(read_byte(address) == value);
	}
}

TEST_CASE("Write byte - restricted area", "[memory][write][byte]") {
	unsigned short address = GENERATE(take(50, random(0xFEA0, 0xFEFF)));
	unsigned char value = GENERATE(take(1, random(0x01, 0xFF)));
	write_byte(address, value);
	DYNAMIC_SECTION("Write attempt to 0x" << std::hex << address) {
		CHECK(read_byte(address) == 0);
	}
}

TEST_CASE("Write short - read-only memory", "[memory][write][short]") {
	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFE)));
	unsigned short word = GENERATE(take(1, random(0x0001, 0xFFFF)));
	ROM_banks[address] = 0x00;
	ROM_banks[address+1] = 0x00;
	write_short(address, word);
	DYNAMIC_SECTION("Write attempt to 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(read_byte(address) == 0);
	}
}

TEST_CASE("Write short - memory", "[memory][write][short]") {
	unsigned short address = GENERATE(take(95, random(0x8000, 0xFE9F)),
					  take(5, random(0xFF00, 0xFFFF)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	write_short(address, word);
	DYNAMIC_SECTION("Write to 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(read_short(address) == word);
	}
}

TEST_CASE("Write short - restricted area", "[memory][write][short]") {
	unsigned short address = GENERATE(take(50, random(0xFEA0, 0xFEFE)));
	unsigned short word = GENERATE(take(1, random(0x0001, 0xFFFF)));
	write_byte(address, word);
	DYNAMIC_SECTION("Write attempt to 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(read_byte(address) == 0);
	}
}

TEST_CASE("Push to stack", "[memory][write][stack]") {
	unsigned short address = GENERATE(take(50, random(0xFF82, 0xFFFF)));
	registers.SP = address;
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	push_short_stack(word);
	CHECK(word == read_short(address-2));
	CHECK(registers.SP == address-2);
}

//endregion
