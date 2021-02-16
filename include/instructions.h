#pragma once

extern int (*instructions[256])(void);

// Base methods
int load_8bit_vp(unsigned char value, unsigned char *storage, int cycles);
int load_8bit_va(unsigned char value, unsigned short address, int cycles);

// Instructions
int ld_b_n(void);
int ld_c_n(void);
int ld_d_n(void);
int ld_e_n(void);
int ld_h_n(void);
int ld_l_n(void);
int ld_b_b(void);
int ld_b_c(void);
int ld_b_d(void);
int ld_b_e(void);
int ld_b_h(void);
int ld_b_l(void);
int ld_b_hl(void);
int ld_c_b(void);
int ld_c_c(void);
int ld_c_d(void);
int ld_c_e(void);
int ld_c_h(void);
int ld_c_l(void);
int ld_c_hl(void);
int ld_d_b(void);
int ld_d_c(void);
int ld_d_d(void);
int ld_d_e(void);
int ld_d_h(void);
int ld_d_l(void);
int ld_d_hl(void);
int ld_a_b(void);
int ld_a_c(void);
int ld_a_d(void);
int ld_a_e(void);
int ld_a_h(void);
int ld_a_l(void);
int ld_a_hl(void);
int ld_a_a(void);