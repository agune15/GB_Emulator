#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "rom.h"
#include "memory.h"

rom_type_t rom_type;
char rom_title[18];
char *save_file_path;
bool cartridge_has_battery = false;

unsigned char current_ROM_bank = 1;
unsigned char current_RAM_bank = 0;

unsigned char cartridge[0x200000] = {0};
unsigned char cartridge_RAM_banks[RAM_BANK_SIZE*4] = {0};

static int load_ROM(char *rom_path);
static size_t get_file_size(FILE *pfile);
static void get_game_title(FILE *pfile);
static void get_save_file_path(char *rom_path);
static rom_type_t get_rom_type(FILE *pfile);
static void load_rom_into_memory(FILE *pfile, size_t file_size);
static void load_saved_game(void);

// Load cartridge from cartridge_path into memory
int load_cartridge(int argc, char *cartridge_path)
{
    if (argc != 1) {
        if(argc < 1)
            printf("rom: No file provided\n");
        else if(argc > 1)
            printf("rom: More than one file was provided\n");

        return 1;
    }

    if (strcmp(strrchr(cartridge_path, '.'), ".gb") != 0) {
        printf("rom: The file provided is not a Game Boy ROM: %s\n", cartridge_path);
        return 1;
    }

    printf("rom: Loading \"%s\"\n", strrchr(cartridge_path, '\\') + 1);

    return load_ROM(cartridge_path);
}

// Load a ROM from rom_path
static int load_ROM(char *rom_path)
{
	FILE *pfile;
	size_t file_size;

    // Open ROM
	if((pfile = fopen(rom_path, "rb")) == NULL) {
		printf("rom: File couldn't be opened");
		return 1;
	}

    // Get file size
	file_size = get_file_size(pfile);

    // Get game title
    get_game_title(pfile);

    // Get save file path
    get_save_file_path(rom_path);

    // Get type
    rom_type = get_rom_type(pfile);

    if (rom_type == UNSUPPORTED) {
        printf("rom: Cartridge type not supported");
        return 1;
    }

    // Load ROM into memory
    load_rom_into_memory(pfile, file_size);

	fclose(pfile);

    // Load saved game
    if (cartridge_has_battery && fopen(save_file_path, "r") != NULL)
        load_saved_game();

    printf("rom: Cartridge loaded successfully\n");

	return 0;
}

// Save the content of the external RAM into disk
void save_game(void) {
    if (rom_type == ROM_ONLY)
        return;

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

    free(save_file_path);
}

//region Helpers

// Get file size from file pointer
static size_t get_file_size(FILE *pfile) {
    fseek(pfile, 0, SEEK_END);
    size_t size = ftell(pfile);
    rewind(pfile);
    return size;
}

// Get game title
static void get_game_title(FILE *pfile) {
    fseek(pfile, ROM_TITLE_OFFSET, SEEK_SET);

    for(int i = 0; ftell(pfile) <= 0x143; i++) {
        char c = fgetc(pfile);

        if (c == 0) {
            c = '\0';
            rom_title[i] = c;
            break;
        }
        else {
            if (c == ' ')
                c = '_';

            rom_title[i] = c;
        }

        if (ftell(pfile) == 0x143) {
            rom_title[i] = c;
            rom_title[i+1] = '\0';
        }
    }

    rewind(pfile);
}

// Get save file path
static void get_save_file_path(char *rom_path) {
    save_file_path = (char *) malloc(sizeof(char) * (strlen(rom_path) + strlen(rom_title) + strlen("_save.sg")));
    strncpy(save_file_path, rom_path, strlen(rom_path) - strlen(strrchr(rom_path, '\\') + 1));
    save_file_path[strlen(rom_path) - strlen(strrchr(rom_path, '\\') + 1)] = '\0';
    strcat(save_file_path, rom_title);
    strcat(save_file_path, "_save.sg");
}

// Get ROM type from ROM type byte
static rom_type_t get_rom_type(FILE *pfile) {
    fseek(pfile, ROM_TYPE_OFFSET, SEEK_SET);
    unsigned char rom_type_byte = fgetc(pfile);
    rewind(pfile);

    if (rom_type_byte == 0)
        return ROM_ONLY;
    else if (rom_type_byte >= 1 && rom_type_byte <= 3) {
        if (rom_type_byte == 3)
            cartridge_has_battery = true;
        return MBC1;
    }
    else if (rom_type_byte >= 5 && rom_type_byte <= 6) {
        if (rom_type_byte == 6)
            cartridge_has_battery = true;
        return MBC2;
    }
    else
        return UNSUPPORTED;
}

// Load ROM content into memory
static void load_rom_into_memory(FILE *pfile, size_t file_size) {
    if (fread(cartridge, 1, file_size, pfile) != file_size)
        printf("rom: Error loading ROM into memory");

    for(int i = 0; i < 0x8000; i++)
        ROM_banks[i] = cartridge[i];
}

// Load the content of the external RAM saved in disk
static void load_saved_game(void) {
    FILE *f = fopen(save_file_path, "r");
    if (f == NULL) {
        printf("rom: Save file path is invalid");
        return;
    }
    printf("rom: Save file path: %s\n", save_file_path);

    char data_line[9];
    int index, value;
    while (fgets(data_line, 9, f) != NULL) {
        sscanf(data_line, "%04X %02X\n", &index, &value);
        cartridge_RAM_banks[index] = value;
    }

    fclose(f);

    printf("rom: Saved game loaded successfully\n");
}

//endregion
