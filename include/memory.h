/*
 *
 */

#pragma once

extern unsigned char ROM_banks[0x8000];
extern unsigned char VRAM[0x2000];
extern unsigned char exRAM[0x2000];
extern unsigned char WRAM[0x2000];
extern unsigned char OAM[0xA0];
extern unsigned char IO[0x80];
extern unsigned char HRAM[0x7F];
extern unsigned char interrupt_enable_reg;

void init_memory();

unsigned char read_byte(unsigned short address);
unsigned short read_short(unsigned short address);
unsigned short pop_short_stack(void);

void write_byte(unsigned short address, unsigned char byte);
void write_short(unsigned short address, unsigned short word);
void push_short_stack(unsigned short word);