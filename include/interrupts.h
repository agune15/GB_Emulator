#pragma once

#define IE_ADDRESS  0xFFFF	// Interrupt Enable register address
#define IF_ADDRESS  0xFF0F	// Interrupt Flag register address

typedef enum {VBLANK, LCD, TIMER, SERIAL, JOYPAD} interrupts_t;

extern bool interrupt_master_enable;

void check_interrupts_state(void);
void request_interrupt(interrupts_t interrupt);
void perform_interrupt(interrupts_t interrupt);
bool is_interrupt_pending(void);