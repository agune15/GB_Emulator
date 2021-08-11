#include <stdio.h>

#include "rom.h"
#include "memory.h"

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
	if(fgetc(pfile) != 0x00) {
		printf("rom: Unsupported cartridge type");
		return 1;
	}

	rom_size = fgetc(pfile);
	if(rom_size == 0x00 && file_size / 1024 != 32) {
		printf("rom: ROM file size isn't equal to the expected ROM size: %dKB != %dKB", file_size / 1024, 32);
		return 1;
	}
	rewind(pfile);

	fread(ROM_banks, 1, file_size, pfile);

	fclose(pfile);

	return 0;
}