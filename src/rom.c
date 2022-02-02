#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "rom.h"
#include "memory.h"

rom_type_t rom_type;
char rom_title[17];
char *save_file_path;   //TODO: Free memory used by it when program terminates
bool cartridge_has_battery = false;

unsigned char current_ROM_bank = 1;
unsigned char current_RAM_bank = 0;

unsigned char cartridge[0x200000] = {0};
unsigned char cartridge_RAM_banks[RAM_BANK_SIZE*4] = {0};

static void load_saved_game(void);

// Load a ROM from rom_path
int load_ROM(char *rom_path)
{
	FILE *pfile;
	size_t file_size;
	unsigned char rom_size;
	unsigned char rom_type_byte;

    // Open ROM
	if((pfile = fopen(rom_path, "rb")) == NULL) {
		printf("rom: File couldn't be opened");
		return 1;
	}

    // Get size
	fseek(pfile, 0, SEEK_END);
	file_size = ftell(pfile);

    // Get title
    fseek(pfile, ROM_TITLE_OFFSET, SEEK_SET);
    while(ftell(pfile) <= 0x143) {
        char c = fgetc(pfile);
        if (c == 0) {
            c = '\0';
            strcat(rom_title, &c);
            break;
        }
        else {
            if (c == ' ')
                c = '_';

            strcat(rom_title, &c);
        }

        if (ftell(pfile) == 0x143) {
            c = '\0';
            strcat(rom_title, &c);
        }
    }

    // Get save file name
    save_file_path = (char *) malloc(sizeof(char) * (strlen(rom_path) + strlen(rom_title) + strlen("_save.sg")));
    strncpy(save_file_path, rom_path, strlen(rom_path) - strlen(strrchr(rom_path, '\\') + 1));
    save_file_path[strlen(rom_path) - strlen(strrchr(rom_path, '\\') + 1)] = '\0';
    strcat(save_file_path, rom_title);
    strcat(save_file_path, "_save.sg");

    // Get type
	fseek(pfile, ROM_TYPE_OFFSET, SEEK_SET);
	rom_type_byte = fgetc(pfile);

    //TODO: Move to separate function
	if (rom_type_byte == 0)
		rom_type = ROM_ONLY;
	else if (rom_type_byte >= 1 && rom_type_byte <= 3) {
        rom_type = MBC1;
        if (rom_type_byte == 3)
            cartridge_has_battery = true;
    }
	else if (rom_type_byte >= 5 && rom_type_byte <= 6) {
        rom_type = MBC2;
        if (rom_type_byte == 6)
            cartridge_has_battery = true;
    }
	if(rom_type_byte > 6) {
		printf("rom: Unsupported cartridge type");
		return 1;
	}

	rewind(pfile);

	fread(cartridge, 1, file_size, pfile);
	for(int i = 0; i < 0x8000; i++)
		ROM_banks[i] = cartridge[i];

	fclose(pfile);

    // Load saved game
    if (cartridge_has_battery && fopen(save_file_path, "r") != NULL)
        load_saved_game();

	return 0;
}

// Save the content of the external RAM into disk
void save_game(void) {
    FILE *f = fopen(save_file_path, "w");
    if (f == NULL) {
        printf("rom: Save file path is invalid");
        return;
    }

    char RAM_value[3], RAM_pos[5];

    for (int i = 0; i < RAM_BANK_SIZE*4; i++) {
        if (cartridge_RAM_banks[i] != 0) {
            sprintf(RAM_pos, "%04X", i);
            fwrite(RAM_pos, sizeof(char), strlen(RAM_pos), f);
            fwrite(" ", sizeof(char), 1, f);
            sprintf(RAM_value, "%02X", cartridge_RAM_banks[i]);
            fwrite(RAM_value, sizeof(char), strlen(RAM_value), f);
            fwrite("\n", sizeof(char), 1, f);
        }
    }
    fclose(f);

    //TODO: Move into its own method
    free(save_file_path);
}

// Load the content of the external RAM saved in disk
static void load_saved_game(void) {
    printf("1\n");
    FILE *f = fopen(save_file_path, "r");
    if (f == NULL) {
        printf("rom: Save file path is invalid");
        return;
    }

    char data_line[8];
    int index, value;
    printf("2\n");
    while (fgets(data_line, 8, f) != NULL) {
        printf("3\n");
        sscanf(data_line, "%04 %02", &index, &value);
        printf("4\n");
        cartridge_RAM_banks[index] = value;
        printf("Index: %04X, value: %02X, cartridge byte: %02X\n", index, value, cartridge_RAM_banks[index]);
    }

    fclose(f);

    printf("rom: Saved game loaded successfully");
}