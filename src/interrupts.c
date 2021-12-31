#include <stdbool.h> //TODO: Could be removed, already in header
#include "interrupts.h"
#include "memory.h"
#include "registers.h"

unsigned short interrupt_addresses[5] = { 0x0040,
					  0x0048,
					  0x0050,
					  0x0058,
					  0x0060 };

bool interrupt_master_enable = false;	//TODO: Include in init() method in the future

// Verify state of interrupt flags and perform any pending interrupts
void check_interrupts_state(void)
{
	if(!interrupt_master_enable)
		return;

	unsigned char current_IE = read_byte(IE_ADDRESS);
	unsigned char current_IF = read_byte(IF_ADDRESS);

	for(int i = 0; i <= JOYPAD; i++) {
		if(current_IF & 0x01) {
			if(current_IE & 0x01) {
				perform_interrupt((interrupts_t)i);
			}
		}
		current_IE = current_IE >> 1;
		current_IF = current_IF >> 1;
	}
}

// Set desired interrupt flag in IF
void request_interrupt(interrupts_t interrupt)
{
	unsigned char interrupt_flags = read_byte(IF_ADDRESS);
	interrupt_flags |= (1 << interrupt);
	write_byte(IF_ADDRESS, interrupt_flags);
}

// Jump to instruction routine of desired interrupt
void perform_interrupt(interrupts_t interrupt)
{
	interrupt_master_enable = false;
	unsigned char current_IF = read_byte(IF_ADDRESS);
	current_IF &= ~(1 << interrupt);
	write_byte(IF_ADDRESS, current_IF);
	push_short_stack(registers.PC);
	registers.PC = interrupt_addresses[interrupt];
}

// Check if an interrupt is pending
bool is_interrupt_pending(void)
{
    bool is_pending = false;

    unsigned char current_IE = read_byte(IE_ADDRESS);
    unsigned char current_IF = read_byte(IF_ADDRESS);

    for(int i = 0; i <= JOYPAD; i++) {
        if(current_IF & 0x01) {
            if(current_IE & 0x01) {
                is_pending = true;
                break;
            }
        }
        current_IE = current_IE >> 1;
        current_IF = current_IF >> 1;
    }

    return is_pending;
}