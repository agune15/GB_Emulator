#pragma once

#define CLOCK_SPEED  4194304
#define CYCLES_FRAME 69905	//TODO: Why is this here?

typedef enum {CARRY = 4, HALFCARRY, NEGATIVE, ZERO} flags_t;

void init_registers(void);
int execute_next_instruction(void);
