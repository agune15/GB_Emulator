#include "catch.hpp"

extern "C" {
	#include "memory.h"
}

//region Read memory tests

TEST_CASE("Read ROM banks", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFF)));
	unsigned char value = ROM_banks[address] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read VRAM", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0x8000, 0x9FFF)));
	unsigned char value = VRAM[address - 0x8000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read exRAM", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0xA000, 0xBFFF)));
	unsigned char value = exRAM[address - 0xA000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read WRAM", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0xC000, 0xDFFF)));
	unsigned char value = WRAM[address - 0xC000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read OAM", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0xFE00, 0xFE9F)));
	unsigned char value = OAM[address - 0xFE00] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read IO", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0xFF00, 0xFF7F)));
	unsigned char value = IO[address - 0xFF00] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read HRAM", "[memory][read]") {
	unsigned short address = GENERATE(take(100, random(0xFF80, 0xFFFE)));
	unsigned char value = HRAM[address - 0xFF80] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read Interrupt Enable register", "[memory][read]") {
	unsigned short address = 0xFFFF;
	unsigned char value = interrupt_enable_reg = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

//endregion

//region Read

TEST_CASE("Write read-only memory", "[memory][write]") {
	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFF)));
	ROM_banks[address] = 0x00;
	unsigned char value = GENERATE(take(1, random(0x01, 0xFF)));
	write_byte(address, value);
	DYNAMIC_SECTION("Write attempt to 0x" << std::hex << address) {
		CHECK(read_byte(address) == 0x00);
	}
}

TEST_CASE("Write memory", "[memory][write]") {
	unsigned short address = GENERATE(take(95, random(0x8000, 0xFE9F)),
				   	  take(5, random(0xFF00, 0xFFFF)));
	unsigned char value = GENERATE(take(1, random(0x00, 0xFF)));
	write_byte(address, value);
	DYNAMIC_SECTION("Write to 0x" << std::hex << address) {
		CHECK(read_byte(address) == value);
	}
}

TEST_CASE("Write restricted area", "[memory][write]") {
	unsigned short address = GENERATE(take(50, random(0xFEA0, 0xFEFF)));
	OAM[address - 0xFE00] = 0x00;
	unsigned char value = GENERATE(take(1, random(0x01, 0xFF)));
	write_byte(address, value);
	DYNAMIC_SECTION("Write attempt to 0x" << std::hex << address) {
		CHECK(read_byte(address) == 0x00);
	}
}

//endregion
