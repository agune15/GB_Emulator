#pragma once

#define ROM_OFFSET_TYPE 0x147

typedef enum {ROM_ONLY, MBC1, MBC2} rom_type_t;

int loadROM(char *rom_path);
