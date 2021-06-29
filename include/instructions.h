#pragma once

//extern int (*instructions[256])(void);

// Instructions
int ld_bc_nn(void);
int ld_bc_a(void);
int inc_bc(void);
int inc_b(void);
int dec_b(void);
int ld_b_n(void);
int ld_nnp_sp(void);
int add_hl_bc(void);
int ld_a_bc(void);
int dec_bc(void);
int inc_c(void);
int dec_c(void);
int ld_c_n(void);
// 0x1-
int ld_de_nn(void);
int ld_de_a(void);
int inc_de(void);
int inc_d(void);
int dec_d(void);
int ld_d_n(void);
int add_hl_de(void);
int ld_a_de(void);
int dec_de(void);
int inc_e(void);
int dec_e(void);
int ld_e_n(void);
// 0x2-
int ld_hl_nn(void);
int ldi_hl_a(void);
int inc_hl(void);
int inc_h(void);
int dec_h(void);
int ld_h_n(void);
int add_hl_hl(void);
int ldi_a_hl(void);
int dec_hl(void);
int inc_l(void);
int dec_l(void);
int ld_l_n(void);
// 0x3-
int ld_sp_nn(void);
int ldd_hl_a(void);
int inc_sp(void);
int inc_hlp(void);
int dec_hlp(void);
int ld_hl_n(void);
int add_hl_sp(void);
int ldd_a_hl(void);
int dec_sp(void);
int inc_a(void);
int dec_a(void);
int ld_a_n(void);
// 0x4-
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
// 0x5-
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
// 0x6-
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
// 0x7-
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
// 0x8-
int add_a_b(void);
int add_a_c(void);
int add_a_d(void);
int add_a_e(void);
int add_a_h(void);
int add_a_l(void);
int add_a_hl(void);
int add_a_a(void);
int adc_a_b(void);
int adc_a_c(void);
int adc_a_d(void);
int adc_a_e(void);
int adc_a_h(void);
int adc_a_l(void);
int adc_a_hl(void);
int adc_a_a(void);
// 0x9-
int sub_a_b(void);
int sub_a_c(void);
int sub_a_d(void);
int sub_a_e(void);
int sub_a_h(void);
int sub_a_l(void);
int sub_a_hl(void);
int sub_a_a(void);
int sbc_a_b(void);
int sbc_a_c(void);
int sbc_a_d(void);
int sbc_a_e(void);
int sbc_a_h(void);
int sbc_a_l(void);
int sbc_a_hl(void);
int sbc_a_a(void);
// 0xA-
int and_a_b(void);
int and_a_c(void);
int and_a_d(void);
int and_a_e(void);
int and_a_h(void);
int and_a_l(void);
int and_a_hl(void);
int and_a_a(void);
int xor_a_b(void);
int xor_a_c(void);
int xor_a_d(void);
int xor_a_e(void);
int xor_a_h(void);
int xor_a_l(void);
int xor_a_hl(void);
int xor_a_a(void);
// OxB-
int or_a_b(void);
int or_a_c(void);
int or_a_d(void);
int or_a_e(void);
int or_a_h(void);
int or_a_l(void);
int or_a_hl(void);
int or_a_a(void);
int cp_a_b(void);
int cp_a_c(void);
int cp_a_d(void);
int cp_a_e(void);
int cp_a_h(void);
int cp_a_l(void);
int cp_a_hl(void);
int cp_a_a(void);
// 0xC-
int pop_bc(void);
int push_bc(void);
int add_a_n(void);
int adc_a_n(void);
// 0xD-
int pop_de(void);
int push_de(void);
int sub_a_n(void);
int sbc_a_n(void);
// 0xE-
int ld_ff_n_a(void);
int pop_hl(void);
int ld_ff_c_a(void);
int push_hl(void);
int and_a_n(void);
int add_sp_n(void);
int ld_nnp_a(void);
int xor_a_n(void);
// 0xF-
int ld_a_ff_n(void);
int pop_af(void);
int ld_a_ff_c(void);
int push_af(void);
int or_a_n(void);
int ld_hl_sp_n(void);
int ld_sp_hl(void);
int ld_a_nnp(void);
int cp_a_n(void);