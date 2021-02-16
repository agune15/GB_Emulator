#pragma once

#include <stdbool.h>

#define CLOCK_SPEED  4194304
#define CYCLES_FRAME 69905

#define FLAG_Z 7
#define FLAG_N 6
#define FLAG_H 5
#define FLAG_C 4

void init_registers(void);
int execute_next_instruction(void);
