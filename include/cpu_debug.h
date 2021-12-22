#pragma once

extern bool is_debugging_CPU;
extern bool debug_next_instruction;

void print_cpu_info(void);
void store_cpu_state_before_opcode(void);
