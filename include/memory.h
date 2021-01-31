/*
 *
 */

#pragma once

extern unsigned char ROM_banks[0x8000];
extern unsigned char VRAM[0x2000];
extern unsigned char exRAM[0x2000];
extern unsigned char WRAM[0x2000];
extern unsigned char OAM[0x100];
extern unsigned char IO[0x80];
extern unsigned char HRAM[0x7F];
extern unsigned char interrupt_enable_reg;

void init_memory();
unsigned char read_byte(unsigned short address);
void write_byte(unsigned short address, unsigned char byte);