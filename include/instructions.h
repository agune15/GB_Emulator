#pragma once

extern int (*instructions[256])(void);

// Instructions
int ld_bc_a(void);
int ld_b_n(void);
int ld_a_bc(void);
int ld_c_n(void);
int ld_de_a(void);
int ld_d_n(void);
int ld_a_de(void);
int ld_e_n(void);
int ld_h_n(void);
int ld_l_n(void);
int ldd_hl_a(void);
int ld_hl_n(void);
int ldd_a_hl(void);
int ld_a_n(void);
int ld_b_b(void);
int ld_b_c(void);
int ld_b_d(void);
int ld_b_e(void);
int ld_b_h(void);
int ld_b_l(void);
int ld_b_hl(void);
int ld_b_a(void);
int ld_c_b(void);
int ld_c_c(void);
int ld_c_d(void);
int ld_c_e(void);
int ld_c_h(void);
int ld_c_l(void);
int ld_c_hl(void);
int ld_c_a(void);
int ld_d_b(void);
int ld_d_c(void);
int ld_d_d(void);
int ld_d_e(void);
int ld_d_h(void);
int ld_d_l(void);
int ld_d_hl(void);
int ld_d_a(void);
int ld_e_b(void);
int ld_e_c(void);
int ld_e_d(void);
int ld_e_e(void);
int ld_e_h(void);
int ld_e_l(void);
int ld_e_hl(void);
int ld_e_a(void);
int ld_h_b(void);
int ld_h_c(void);
int ld_h_d(void);
int ld_h_e(void);
int ld_h_h(void);
int ld_h_l(void);
int ld_h_hl(void);
int ld_h_a(void);
int ld_l_b(void);
int ld_l_c(void);
int ld_l_d(void);
int ld_l_e(void);
int ld_l_h(void);
int ld_l_l(void);
int ld_l_hl(void);
int ld_l_a(void);
int ld_hl_b(void);
int ld_hl_c(void);
int ld_hl_d(void);
int ld_hl_e(void);
int ld_hl_h(void);
int ld_hl_l(void);
int ld_hl_a(void);
int ld_a_b(void);
int ld_a_c(void);
int ld_a_d(void);
int ld_a_e(void);
int ld_a_h(void);
int ld_a_l(void);
int ld_a_hl(void);
int ld_a_a(void);
int ld_ff_c_a(void);
int ld_nn_a(void);
int ld_a_ff_c(void);
int ld_a_nn(void);