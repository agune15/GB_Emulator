#include <stdio.h>

#include "rom.h"
#include "memory.h"

unsigned char rom_type_byte;
rom_type_t rom_type;
unsigned char current_ROM_bank = 1;
unsigned char current_RAM_bank = 0;

unsigned char cartridge[0x200000] = {0};
unsigned char cartridge_RAM_banks[0x8000] = {0};

// Load a ROM from rom_path
int loadROM(char *rom_path)
{
	FILE *pfile;
	size_t file_size;
	unsigned char rom_size;

	if((pfile = fopen(rom_path, "rb")) == NULL) {
		printf("rom: File couldn't be opened");
		return 1;
	}

	fseek(pfile, 0, SEEK_END);
	file_size = ftell(pfile);

	fseek(pfile, ROM_OFFSET_TYPE, SEEK_SET);
	rom_type_byte = fgetc(pfile);

	if (rom_type_byte == 0)
		rom_type = ROM_ONLY;
	else if (rom_type_byte >= 1 && rom_type_byte <= 3)
		rom_type = MBC1;
	else if (rom_type_byte >= 5 && rom_type_byte <= 6)
		rom_type = MBC2;
	if(rom_type_byte > 6) {
		printf("rom: Unsupported cartridge type");
		return 1;
	}

	rom_size = fgetc(pfile);
	if(rom_size == 0x00 && file_size / 1024 != 32) {
		printf("rom: ROM file size isn't equal to the expected ROM size: %dKB != %dKB", file_size / 1024, 32);
		return 1;
	}
	rewind(pfile);

	fread(cartridge, 1, file_size, pfile);
	for(int i = 0; i < 0x8000; i++)
		ROM_banks[i] = cartridge[i];

	fclose(pfile);

	return 0;
}
