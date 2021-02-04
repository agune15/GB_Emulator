#pragma once

#define IE_ADDRESS  0xFFFF	// Interrupt Enable register address
#define IF_ADDRESS  0xFF0F	// Interrupt Flag register address

#define INTERRUPT_VBLANK_BIT  0
#define INTERRUPT_LCD__BIT    1
#define INTERRUPT_TIMER_BIT   2
#define INTERRUPT_JOYPAD_BIT  4

#define INTERRUPT_VBLANK_ADDRESS  0x0040	//TODO: Change address for something else related to the Program Counter
#define INTERRUPT_LCD_ADDRESS     0x0048
#define INTERRUPT_TIMER_ADDRESS   0x0050
#define INTERRUPT_JOYPAD_ADDRESS  0x0060

void request_interrupt(unsigned char interrupt_flag_bit);
void check_interrupts_state(void);
void perform_interrupt(unsigned char interrupt_flag_bit);