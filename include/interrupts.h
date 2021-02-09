#pragma once

#define IE_ADDRESS  0xFFFF	// Interrupt Enable register address
#define IF_ADDRESS  0xFF0F	// Interrupt Flag register address

typedef enum {VBLANK, LCD, TIMER, SERIAL, JOYPAD} interrupt_t;

void request_interrupt(interrupt_t interrupt);
void check_interrupts_state(void);
void perform_interrupt(interrupt_t interrupt);