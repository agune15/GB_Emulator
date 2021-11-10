#include <stdio.h>
#include "memory.h"
#include "input.h"
#include "registers.h"
#include "time.h"
#include "stdlib.h"

unsigned char ROM_banks[0x8000];	//0000-7FFF
unsigned char VRAM[0x2000];		//8000-9FFF
unsigned char exRAM[0x2000];		//A000-BFFF
unsigned char WRAM[0x2000];		//C000-DFFF (& E000-FDFF, same as C000-DDFF)
unsigned char OAM[0xA0];		//FE00-FE9F
unsigned char IO[0x80];			//FF00-FF7F
unsigned char HRAM[0x7F];		//FF80-FFFE
unsigned char interrupt_enable_reg;	//FFFF

static void perform_DMA_transfer(unsigned short address);

// Assign the required initial value for a set of specific memory addresses (GB requirement)
void init_memory(void)
{
	write_byte(0xFF05, 0x00);
	write_byte(0xFF06, 0x00);
	write_byte(0xFF07, 0x00);
	write_byte(0xFF10, 0x80);
	write_byte(0xFF11, 0xBF);
	write_byte(0xFF12, 0xF3);
	write_byte(0xFF14, 0xBF);
	write_byte(0xFF16, 0x3F);
	write_byte(0xFF17, 0x00);
	write_byte(0xFF19, 0xBF);
	write_byte(0xFF1A, 0x7F);
	write_byte(0xFF1B, 0xFF);
	write_byte(0xFF1C, 0x9F);
	write_byte(0xFF1E, 0xBF);
	write_byte(0xFF20, 0xFF);
	write_byte(0xFF21, 0x00);
	write_byte(0xFF22, 0x00);
	write_byte(0xFF23, 0xBF);
	write_byte(0xFF24, 0x77);
	write_byte(0xFF25, 0xF3);
	write_byte(0xFF26, 0xF1);
	write_byte(0xFF40, 0x91);
	write_byte(0xFF42, 0x00);
	write_byte(0xFF43, 0x00);
	write_byte(0xFF45, 0x00);
	write_byte(0xFF47, 0xFC);
	write_byte(0xFF48, 0xFF);
	write_byte(0xFF49, 0xFF);
	write_byte(0xFF4A, 0x00);
	write_byte(0xFF4B, 0x00);
	write_byte(0xFFFF, 0x00);
}

// Initialize random seed
void init_random_seed(void)
{
	srand(time(NULL));
}

// Read byte from address
unsigned char read_byte(unsigned short address)
{
	if (address <= 0x7FFF)
		return ROM_banks[address];
	else if (address >= 0x8000 && address <= 0x9FFF)
		return VRAM[address - 0x8000];
	else if (address >= 0xA000 && address <= 0xBFFF)
		return exRAM[address - 0xA000];
	else if (address >= 0xC000 && address <= 0xFDFF)
		return WRAM[address - 0xC000];
	else if (address >= 0xFE00 && address <= 0xFE9F)
		return OAM[address - 0xFE00];
	else if (address >= 0xFF00 && address <= 0xFF7F) {
		if (address == 0xFF00)		//TODO: This address to a macro
			return get_joypad_state(IO[0]);
		else if (address == 0xFF04)		//TODO: This address to a macro
			return (unsigned char)rand();
		else
			return IO[address - 0xFF00];
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
		return HRAM[address - 0xFF80];
	else if (address == 0xFFFF)
		return interrupt_enable_reg;

	return 0;
}

// Read short (2 bytes) from address
unsigned short read_short(unsigned short address)
{
	return read_byte(address) + (read_byte(address + 1) << 8);
}

// Pop short (2 bytes) from stack and change SP accordingly
unsigned short pop_short_stack(void)
{
	unsigned short word = read_short(registers.SP);
	registers.SP += 2;
	return word;
}

// Override byte value from address
void write_byte(unsigned short address, unsigned char byte)
{
	if (address >= 0xFEA0 && address <= 0xFEFF) {
		//printf("memory: Restricted memory area: %#x\n", address);
		//TODO: Uncomment
		return;
	}

	//if (address >= 0x0000 && address <= 0x7FFF)
		//printf("memory: Read-only memory area: %#x\n", address);
		//TODO: Uncomment
	else if (address >= 0x8000 && address <= 0x9FFF)
		VRAM[address - 0x8000] = byte;
	else if (address >= 0xA000 && address <= 0xBFFF)
		exRAM[address - 0xA000] = byte;
	else if (address >= 0xC000 && address <= 0xFDFF) {
		if (address < 0xE000)
			WRAM[address - 0xC000] = byte;
		else
			WRAM[address - 0xE000] = byte;
	}
	else if (address >= 0xFE00 && address <= 0xFE9F)
		OAM[address - 0xFE00] = byte;
	else if (address >= 0xFF00 && address <= 0xFF7F) {
		if (address == 0xFF44)
			IO[address - 0xFF00] = 0;
		else if (address == 0xFF46)
			perform_DMA_transfer(byte);
		else
			IO[address - 0xFF00] = byte;
	}
	else if (address >= 0xFF80 && address <= 0xFFFE)
		HRAM[address - 0xFF80] = byte;
	else if (address == 0xFFFF)
		interrupt_enable_reg = byte;
	//else
		//printf("memory: Address unreachable: %#x", address);
		//TODO: Uncomment
}

// Override byte value from address and subsequent address (address + 1)
void write_short(unsigned short address, unsigned short word)
{
	write_byte(address, word & 0x00FF);
	write_byte(address + 1, (word & 0xFF00) >> 8);
}

// Push short (2 bytes) to stack and change SP accordingly
void push_short_stack(unsigned short word)
{
	registers.SP -= 2;
	write_short(registers.SP, word);
}

// Perform DMA transfer from address to OAM
static void perform_DMA_transfer(unsigned short address)
{
	address <<= 8;
	for (int i = 0; i < 0xA0; i++)
		write_byte(0xFE00+i, read_byte(address + i));
}