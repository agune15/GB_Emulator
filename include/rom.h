#pragma once

#define ROM_TITLE_OFFSET 0x134
#define ROM_TYPE_OFFSET  0x147
#define ROM_BANK_SIZE    0x4000
#define RAM_BANK_SIZE    0x2000

typedef enum {ROM_ONLY, MBC1, MBC2} rom_type_t;
extern rom_type_t rom_type;
extern bool cartridge_has_battery;

extern unsigned char current_ROM_bank;
extern unsigned char current_RAM_bank;

extern unsigned char cartridge[0x200000];
extern unsigned char cartridge_RAM_banks[0x8000];

int load_ROM(char *rom_path);
void save_game(void);
