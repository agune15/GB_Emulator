#pragma once

#include <stdbool.h>

#define IE_ADDRESS  0xFFFF	// Interrupt Enable register address
#define IF_ADDRESS  0xFF0F	// Interrupt Flag register address

typedef enum {VBLANK, LCD, TIMER, SERIAL, JOYPAD} interrupts_t;

extern bool interrupt_master_enable;

void request_interrupt(interrupts_t interrupt);
void check_interrupts_state(void);
void perform_interrupt(interrupts_t interrupt);