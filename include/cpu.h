#pragma once

#include <stdbool.h>

#define CLOCK_SPEED  4194304
#define CYCLES_FRAME 69905	//TODO: Why is this here? Needed? Move elsewhere?

typedef enum {CARRY = 4, HALFCARRY, NEGATIVE, ZERO} flags_t;

void init_registers(void);
int execute_next_instruction(void);
void resume_cpu(void);

bool is_flag_set(flags_t flag);
void set_flag(flags_t flag);
void reset_flag(flags_t flag);
