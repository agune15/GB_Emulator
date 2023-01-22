#include "catch.hpp"

extern "C" {
	#include "../src/memory.c"
}

void MBC1_RAM_enable_test(unsigned short address);
void read_ROM_banks_MBC1_test();

//region Read from memory

TEST_CASE("Read ROM banks - ROM ONLY", "[memory][read][byte]") {
    rom_type = ROM_ONLY;

	unsigned short address = GENERATE(take(100, random(0x0000, 0x7FFF)));
	unsigned char value = ROM_banks[address] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read ROM banks - MBC1", "[memory][read][byte]") {
    rom_type = MBC1;
    current_ROM_bank = GENERATE(take(10, random(1, 0x7F)));

    read_ROM_banks_MBC1_test();
}

TEST_CASE("Read ROM banks - MBC2", "[memory][read][byte]") {
    rom_type = MBC2;
    current_ROM_bank = GENERATE(take(10, random(1, 0x0F)));

    read_ROM_banks_MBC1_test();
}

TEST_CASE("Read VRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(10, random(0x8000, 0x9FFF)));
	unsigned char value = VRAM[address - 0x8000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read exRAM - ROM ONLY", "[memory][read][byte]") {
    rom_type = ROM_ONLY;

	unsigned short address = GENERATE(take(10, random(0xA000, 0xBFFF)));
	unsigned char value = exRAM[address - 0xA000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read exRAM - MBC1", "[memory][read][byte]") {
    rom_type = MBC1;
    is_RAM_w_enabled = true;
    current_RAM_bank = GENERATE(take(4, random(0,3)));

    unsigned short address = GENERATE(take(10, random(0xA000, 0xBFFF)));
    unsigned char value = cartridge_RAM_banks[address - 0xA000 + current_RAM_bank*RAM_BANK_SIZE] = GENERATE(take(1, random(0x00, 0xFF)));
    DYNAMIC_SECTION("Read 0x" << std::hex << address) {
        CHECK(value == read_byte(address));
    }
}

TEST_CASE("Read exRAM - MBC2", "[memory][read][byte]") {
    rom_type = MBC2;
    is_RAM_w_enabled = true;
    current_RAM_bank = 0;

    unsigned short address = GENERATE(take(10, random(0xA000, 0xBFFF)));
    unsigned char value = cartridge_RAM_banks[address - 0xA000 + current_RAM_bank*RAM_BANK_SIZE] = GENERATE(take(1, random(0x00, 0xFF)));
    DYNAMIC_SECTION("Read 0x" << std::hex << address) {
        CHECK(value == read_byte(address));
    }
}

TEST_CASE("Read WRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(10, random(0xC000, 0xDFFF)));
	unsigned char value = WRAM[address - 0xC000] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read OAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(10, random(0xFE00, 0xFE9F)));
	unsigned char value = OAM[address - 0xFE00] = GENERATE(take(1, random(0x00, 0xFF)));
	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read IO", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(10, random(0xFF00, 0xFF7F)));
	if (address == 0xFF04)
		address++;

	unsigned char value = IO[address - 0xFF00] = GENERATE(take(1, random(0x00, 0xFF)));
	if(address == 0xFF00)
		value = get_joypad_state(IO[0]);

	DYNAMIC_SECTION("Read 0x" << std::hex << address) {
		CHECK(value == read_byte(address));
	}
}

TEST_CASE("Read HRAM", "[memory][read][byte]") {
	unsigned short address = GENERATE(take(10, random(0xFF80, 0xFFFE)));
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

TEST_CASE("Read short - read-only memory - ROM ONLY", "[memory][read][short]") {
    rom_type = ROM_ONLY;

	unsigned short address = GENERATE(take(10, random(0x0000, 0x7FFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	ROM_banks[address] = word & 0x00FF;
	ROM_banks[address+1] = (word & 0xFF00) >> 8;
	DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(word == read_short(address));
	}
}

TEST_CASE("Read short - read-only memory - MBC1", "[memory][read][short]") {
    rom_type = MBC1;
    current_ROM_bank = GENERATE(take(10, random(1, 0x7F)));

    unsigned short address = GENERATE(take(10, random(ROM_BANK_SIZE, 0x7FFE)));
    unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
    cartridge[address - ROM_BANK_SIZE + current_ROM_bank*ROM_BANK_SIZE] = word & 0x00FF;
    cartridge[address+1 - ROM_BANK_SIZE + current_ROM_bank*ROM_BANK_SIZE] = (word & 0xFF00) >> 8;
    DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
        CHECK(word == read_short(address));
    }
}

TEST_CASE("Read short - read-only memory - MBC2", "[memory][read][short]") {
    rom_type = MBC2;
    current_ROM_bank = GENERATE(take(10, random(1, 0x0F)));

    unsigned short address = GENERATE(take(10, random(ROM_BANK_SIZE, 0x7FFE)));
    unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
    cartridge[address - ROM_BANK_SIZE + current_ROM_bank*ROM_BANK_SIZE] = word & 0x00FF;
    cartridge[address+1 - ROM_BANK_SIZE + current_ROM_bank*ROM_BANK_SIZE] = (word & 0xFF00) >> 8;
    DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
        CHECK(word == read_short(address));
    }
}

TEST_CASE("Read short - memory (excluding exRAM)", "[memory][read][short]") {
	unsigned short address = GENERATE(take(5, random(0x8000, 0x9FFE)),
                         take(5, random(0xC000, 0xFE9E)),
                         take(5, random(0xFF00, 0xFFFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));

	// Corner cases
	if (address == 0xFF44) word &= 0xFF00;
	else if (address+1 == 0xFF44) word &= 0x00FF;
	else if (address == 0xFF00 || address == 0xFF04 || address == 0xFF46) address++;
	else if (address+1 == 0xFF00 || address+1 == 0xFF04 || address+1 == 0xFF46) address += 2;

	write_short(address, word);
	DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(word == read_short(address));
	}
}

TEST_CASE("Read short - exRAM", "[memory][read][short]") {
    unsigned short address = GENERATE(take(10, random(0xA000, 0xBFFE)));
    unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));

    rom_type = ROM_ONLY;
    write_short(address, word);
    DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
        CHECK(word == read_short(address));
    }

    rom_type = MBC1;
    current_RAM_bank = GENERATE(take(4, random(0,3)));
    write_short(address, word);
    DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
        CHECK(word == read_short(address));
    }

    rom_type = MBC2;
    current_RAM_bank = 0;
    write_short(address, word);
    DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
        CHECK(word == read_short(address));
    }
}

TEST_CASE("Read short - restricted area", "[memory][read][short]") {
	unsigned short address = GENERATE(take(10, random(0xFEA0, 0xFEFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	write_short(address, word);
	DYNAMIC_SECTION("Read 0x"<<std::hex<<address<<" & 0x"<<std::hex<<address+1) {
		CHECK(read_short(address) == 0);
	}
}

TEST_CASE("Pop from stack", "[memory][read][stack]") {
	unsigned short address = GENERATE(take(10, random(0xFF82, 0xFFFE)));
	registers.SP = address;
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));
	push_short_stack(word);
	CHECK(word == pop_short_stack());
	CHECK(registers.SP == address);
}

//endregion

//region Write to memory

TEST_CASE("Write byte - read-only memory - ROM ONLY", "[memory][write][byte]") {
    rom_type = ROM_ONLY;

	unsigned short address = GENERATE(take(10, random(0x0000, 0x7FFF)));
	unsigned char value = GENERATE(take(1, random(0x01, 0xFF)));
	ROM_banks[address] = 0x00;
	write_byte(address, value);
	DYNAMIC_SECTION("Write attempt to 0x" << std::hex << address) {
		CHECK(read_byte(address) == 0);
	}
}

TEST_CASE("Enable/disable RAM writes - MBC1", "[memory][write][byte][banking]") {
    rom_type = MBC1;
    is_RAM_w_enabled = false;

    unsigned short address = GENERATE(take(10, random(0x0000, 0x1FFF)));
    MBC1_RAM_enable_test(address);
}

TEST_CASE("Enable/disable RAM writes - MBC2", "[memory][write][byte][banking]") {
    rom_type = MBC2;
    is_RAM_w_enabled = false;
    bool RAM_w_state = is_RAM_w_enabled;

    unsigned short address = GENERATE(take(10, random(0x0000, 0x1FFF)));

    if ((address >> 4) & 1)
        CHECK(is_RAM_w_enabled == RAM_w_state);
    else
        MBC1_RAM_enable_test(address);
}

TEST_CASE("Change ROM bank 0-4 bits - MBC1", "[memory][write][byte][banking]") {
    rom_type = MBC1;
    current_ROM_bank = GENERATE(take(2, random(0, 0x7F)));

    unsigned short address = GENERATE(take(5, random(0x2000, 0x3FFF)));
    unsigned char value = GENERATE(take(5, random(0x00, 0x7F)));
    unsigned char c_ROM_bank = (value & 0x1F) | (current_ROM_bank & 0xE0);

    write_byte(address, value);

    if (!c_ROM_bank)
        CHECK(current_ROM_bank == 1);
    else
        CHECK(current_ROM_bank == c_ROM_bank);
}

TEST_CASE("Change ROM bank 0-3 bits - MBC2", "[memory][write][byte][banking]") {
    rom_type = MBC2;
    current_ROM_bank = GENERATE(take(2, random(0, 0x0F)));

    unsigned short address = GENERATE(take(5, random(0x2000, 0x3FFF)));
    unsigned char value = GENERATE(take(5, random(0x00, 0x0F)));

    write_byte(address, value);

    if (!value)
        CHECK(current_ROM_bank == 1);
    else
        CHECK(current_ROM_bank == value);
}

TEST_CASE("Change ROM bank 5-7 bits - MBC1", "[memory][write][byte][banking]") {
    rom_type = MBC1;
    is_ROM_banking_enabled = true;
    current_ROM_bank = GENERATE(take(2, random(0, 0x7F)));

    unsigned short address = GENERATE(take(5, random(0x4000, 0x5FFF)));
    unsigned char value = GENERATE(take(5, random(0x00, 0x7F)));
    unsigned char c_ROM_bank = (value & 0xE0) | (current_ROM_bank & 0x1F);

    write_byte(address, value);

    if (!c_ROM_bank)
        CHECK(current_ROM_bank == 1);
    else
        CHECK(current_ROM_bank == c_ROM_bank);
}

TEST_CASE("Change RAM bank - MBC1", "[memory][write][byte][banking]") {
    rom_type = MBC1;
    is_ROM_banking_enabled = false;
    current_RAM_bank = GENERATE(take(2, random(0, 3)));

    unsigned short address = GENERATE(take(5, random(0x4000, 0x5FFF)));
    unsigned char value = GENERATE(take(5, random(0x00, 0xFF)));

    write_byte(address, value);

    CHECK(current_RAM_bank == (value & 3));
}

TEST_CASE("Select banking mode - MBC1", "[memory][write][byte][banking]") {
    rom_type = MBC1;
    current_RAM_bank = GENERATE(take(2, random(1, 3)));

    unsigned short address = GENERATE(take(5, random(0x6000, 0x7FFF)));
    unsigned char value = GENERATE(take(5, random(0x00, 0xFF)));

    write_byte(address, value);

    if (!(value & 1)) {
        CHECK(is_ROM_banking_enabled == true);
        CHECK(current_RAM_bank == 0);
    }
    else
        CHECK(is_ROM_banking_enabled == false);
}

TEST_CASE("Write byte - memory", "[memory][write][byte]") {
    rom_type = ROM_ONLY;

	unsigned short address = GENERATE(take(10, random(0x8000, 0xFE9F)),
				   	  take(5, random(0xFF00, 0xFFFF)));
	unsigned char value = GENERATE(take(1, random(0x00, 0xFF)));

	// Corner cases
	if (address == 0xFF44) value = 0;
	else if (address == 0xFF00 || address == 0xFF04 || address == 0xFF46) address++;

	write_byte(address, value);
	DYNAMIC_SECTION("Write to 0x" << std::hex << address) {
		CHECK(read_byte(address) == value);
	}
}

TEST_CASE("Write byte - exRAM - MBC1-2", "[memory][write][byte]") {
    rom_type = MBC1;
    current_RAM_bank = GENERATE(take(2, random(0, 3)));

    unsigned short address = GENERATE(take(10, random(0xA000, 0xBFFF)));
    unsigned char value = GENERATE(take(1, random(0x00, 0xFF)));
    cartridge_RAM_banks[address - 0xA000 + current_RAM_bank*RAM_BANK_SIZE];

    write_byte(address, value);
    DYNAMIC_SECTION("Write to 0x" << std::hex << address) {
        CHECK(read_byte(address) == value);
    }
}

TEST_CASE("Write byte - restricted area", "[memory][write][byte]") {
	unsigned short address = GENERATE(take(10, random(0xFEA0, 0xFEFF)));
	unsigned char value = GENERATE(take(1, random(0x01, 0xFF)));
	write_byte(address, value);
	DYNAMIC_SECTION("Write attempt to 0x" << std::hex << address) {
		CHECK(read_byte(address) == 0);
	}
}

TEST_CASE("Write short - read-only memory", "[memory][write][short]") {
    rom_type = ROM_ONLY;

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
    rom_type = ROM_ONLY;

	unsigned short address = GENERATE(take(95, random(0x8000, 0xFE9F)),
					  take(5, random(0xFF00, 0xFFFE)));
	unsigned short word = GENERATE(take(1, random(0x0000, 0xFFFF)));

	//Corner cases
	if(address == 0xFF44) word &= 0xFF00;
	else if (address+1 == 0xFF44) word &= 0x00FF;
	else if (address == 0xFF00 || address == 0xFF04 || address == 0xFF46) address++;
	else if (address+1 == 0xFF00 || address+1 == 0xFF04 || address+1 == 0xFF46) address += 2;

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

TEST_CASE("Perform DMA transfer", "[memory][write][DMA]") {
	for(int i = 0; i < 0xA0; i++)
		ROM_banks[i] = GENERATE(take(1, random(0, 0xFF)));
	write_byte(0xFF46, 0x00);
	for(int i = 0; i < 0xA0; i++)
		CHECK(read_byte(i) == read_byte(0xFE00+i));
}

//endregion

// Helpers

void MBC1_RAM_enable_test(unsigned short address)
{
    unsigned char value = 0x0A;
    write_byte(address, value);
    SECTION("Enable RAM writes") {
        CHECK(is_RAM_w_enabled == true);
    }

    value = 0x00;
    write_byte(address, value);
    SECTION("Disable RAM writes") {
        CHECK(is_RAM_w_enabled == false);
    }
}

void read_ROM_banks_MBC1_test()
{
    unsigned short address = GENERATE(take(10, random(ROM_BANK_SIZE, 0x7FFF)));
    unsigned char value = cartridge[address - ROM_BANK_SIZE + current_ROM_bank*ROM_BANK_SIZE] = GENERATE(take(1, random(0x00, 0xFF)));
    DYNAMIC_SECTION("Read 0x" << std::hex << address) {
        CHECK(value == read_byte(address));
    }
}
