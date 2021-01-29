/*
 *
 */

#include <stdio.h>
#include "memory.h"

unsigned char ROM_banks[0x8000];	//0000-7FFF
unsigned char VRAM[0x2000];		//8000-9FFF
unsigned char exRAM[0x2000];		//A000-BFFF
unsigned char WRAM[0x2000];		//C000-DFFF (& E000-FDFF (same as C000-DDFF)
unsigned char OAM[0x100];		//FE00-FE9F
unsigned char IO[0x80];			//FF00-FF7F
unsigned char HRAM[0x7F];		//FF80-FFFE
unsigned char interrupt_enable_reg;	//FFFF

// Assign the required initial value for a set of specific memory addresses (GB requirement)
void init_memory(void)
{
	IO[0x05] = 0x00;
	IO[0x06] = 0x00;
	IO[0x07] = 0x00;
	IO[0x10] = 0x80;
	IO[0x11] = 0xBF;
	IO[0x12] = 0xF3;
	IO[0x14] = 0xBF;
	IO[0x16] = 0x3F;
	IO[0x17] = 0x00;
	IO[0x19] = 0xBF;
	IO[0x1A] = 0x7F;
	IO[0x1B] = 0xFF;
	IO[0x1C] = 0x9F;
	IO[0x1E] = 0xBF;
	IO[0x20] = 0xFF;
	IO[0x21] = 0x00;
	IO[0x22] = 0x00;
	IO[0x23] = 0xBF;
	IO[0x24] = 0x77;
	IO[0x25] = 0xF3;
	IO[0x26] = 0xF1;
	IO[0x40] = 0x91;
	IO[0x42] = 0x00;
	IO[0x43] = 0x00;
	IO[0x45] = 0x00;
	IO[0x47] = 0xFC;
	IO[0x48] = 0xFF;
	IO[0x49] = 0xFF;
	IO[0x4A] = 0x00;
	IO[0x4B] = 0x00;
	interrupt_enable_reg = 0x00;
}

// Read the byte of an address
unsigned char read_byte(unsigned short address)
{
	if(address <= 0x7FFF)
		return ROM_banks[address];
	else if(address >= 0x8000 && address <= 0x9FFF)
		return VRAM[address - 0x8000];
	else if(address >= 0xA000 && address <= 0xBFFF)
		return exRAM[address - 0xA000];
	else if(address >= 0xC000 && address <= 0xDFFF) {
		if(address < 0xE000)
			return WRAM[address - 0xC000];
		else
			return WRAM[address - 0xE000];
	}
	else if(address >= 0xFE00 && address <= 0xFE9F)
		return OAM[address - 0xFE00];
	else if(address >= 0xFF00 && address <= 0xFF7F)
		return IO[address - 0xFE00];
	else if(address >= 0xFF80 && address <= 0xFFFE)
		return HRAM[address - 0xFF80];
	else if(address == 0xFFFF)
		return interrupt_enable_reg;
	else {
		printf("memory: Address unreachable: %#x", address);
	}

	return 0;
}

// Override byte value of address
void write_byte(unsigned short address, unsigned char byte)
{
	if(address >= 0xFEA0 && address <= 0xFEFF) {
		printf("memory: Restricted memory area: %#x", address);
		return;
	}

	if(address >= 0x0000 && address <= 0x7FFF)
		printf("memory: Read-only memory area: %#x", address);
	else if(address >= 0x8000 && address <= 0x9FFF)
		VRAM[address - 0x8000] = byte;
	else if(address >= 0xA000 && address <= 0xBFFF)
		exRAM[address - 0xA000] = byte;
	else if(address >= 0xC000 && address <= 0xDFFF) {
		if(address < 0xE000)
			WRAM[address - 0xC000] = byte;
		else
			WRAM[address - 0xE000] = byte;
	}
	else if(address >= 0xFE00 && address <= 0xFE9F)
		OAM[address - 0xFE00] = byte;
	else if(address >= 0xFF00 && address <= 0xFF7F)
		IO[address - 0xFE00] = byte;
	else if(address >= 0xFF80 && address <= 0xFFFE)
		HRAM[address - 0xFF80] = byte;
	else if(address == 0xFFFF)
		interrupt_enable_reg = byte;
	else
		printf("memory: Address unreachable: %#x", address);

}
