/*
 *
 *
 *
 *
 *
 *
 */

#include <stdio.h>

#include "rom.h"

bool loadROM(char *rom_path)
{
	FILE *pfile;

	if((pfile = fopen(rom_path, "rb")) == NULL) {
		printf("rom: File couldn't be opened");
		return false;
	}

	fseek(pfile, ROM_OFFSET_TYPE, SEEK_SET);
	if(fgetc(pfile) != 0x00) {
		printf("rom: Unsupported cartridge type");
	}
	rewind(pfile);

	//TODO: Compare ROM size and type
	//TODO: Load ROM content into RAM

	fclose(pfile);

	return true;
}