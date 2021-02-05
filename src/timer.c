/*
 *
 */

#include <stdbool.h>

#include "timer.h"
#include "timer_p.h"
#include "memory.h"
#include "cpu.h"
#include "interrupts.h"

int timer_counter = 0;

static bool is_clock_enabled(void);
static void reset_timer_counter(void);

// Update timer with current instruction cycles
void update_timer(int cycles)	//TODO: Dissect function into a few smaller ones
{
	if(!is_clock_enabled())
		return;

	if((timer_counter -= cycles) <= 0) {
		reset_timer_counter();

		if(read_byte(TIMA_ADDRESS) == 0xFF) {
			write_byte(TIMA_ADDRESS, read_byte(TMA_ADDRESS));
			request_interrupt(INTERRUPT_TIMER_BIT);
		}
		else
			write_byte(TIMA_ADDRESS, read_byte(TIMA_ADDRESS) + 1);
	}
}

// Check state of timer's clock (3rd bit of TAC)
static bool is_clock_enabled(void)
{
	return (read_byte(TAC_ADDRESS) & 0x04) ? true : false;
}

// Reset timer_counter with desired frequency
static void reset_timer_counter(void)
{
	int current_freq = read_byte(TAC_ADDRESS) & 0x03;

	switch (current_freq) {
		case 0:
			timer_counter = CLOCK_SPEED/4096;
			break;
		case 1:
			timer_counter = CLOCK_SPEED/262144;
			break;
		case 2:
			timer_counter = CLOCK_SPEED/65536;
			break;
		case 3:
			timer_counter = CLOCK_SPEED/16384;
			break;
		default:
			break;
	}
}

//region Internal helpers

// (Testing only) Check state of timer's clock (3rd bit of TAC)
bool is_clock_enabled_internal(void)
{
	return is_clock_enabled();
}

// (Testing only) Reset timer_counter with desired frequency
void reset_timer_counter_internal(void)
{
	reset_timer_counter();
}

//endregion

