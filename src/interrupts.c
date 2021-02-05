/*
 *
 */

#include <stdbool.h>

#include "interrupts.h"
#include "interrupts_p.h"
#include "memory.h"
#include "cpu.h"

unsigned short interrupt_addresses[5] = { 0x0040,
					  0x0048,
					  0x0050,
					  0x0058,
					  0x0060 };

// Set desired interrupt flag in IF
void request_interrupt(unsigned char interrupt_flag_bit)
{
	unsigned char interrupt_flags = read_byte(IF_ADDRESS);
	interrupt_flags |= (1 << interrupt_flag_bit);
	write_byte(IF_ADDRESS, interrupt_flags);
}

// Verify state of interrupt flags and perform any pending interrupts
void check_interrupts_state(void)
{
	if(!interrupt_master_enable)
		return;

	unsigned char current_IE = read_byte(IE_ADDRESS);
	unsigned char current_IF = read_byte(IF_ADDRESS);

	for(int i = 0; i <= INTERRUPT_JOYPAD_BIT; i++) {
		if(current_IF & 0x01) {
			if(current_IE & 0x01) {
				perform_interrupt(i);
			}
		}
		current_IE = current_IE >> 1;
		current_IF = current_IF >> 1;
	}
}

// Jump to instruction routine of desired interrupt
void perform_interrupt(unsigned char interrupt_flag_bit)
{
	interrupt_master_enable = false;
	unsigned char current_IF = read_byte(IF_ADDRESS);
	current_IF &= ~(1 << interrupt_flag_bit);
	write_byte(IF_ADDRESS, current_IF);
	push_short_stack(registers.PC);
	registers.PC = interrupt_addresses[interrupt_flag_bit];
}