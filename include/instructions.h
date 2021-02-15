#pragma once

extern int (*instructions[256])(void);

// Base methods
int load_8bit_vp(unsigned short value, unsigned short *storage, int cycles);
int load_8bit_va(unsigned short value, unsigned short address, int cycles);

// Instructions
int ld_b_n(void);
int ld_c_n(void);
int ld_d_n(void);
int ld_e_n(void);
int ld_h_n(void);
int ld_l_n(void);
int ld_a_b(void);
int ld_a_c(void);
int ld_a_d(void);
int ld_a_a(void);