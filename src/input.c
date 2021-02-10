/*
 *
 */

#include <stdbool.h>
#include "input.h"
#include "interrupts.h"
#include "memory.h"

#define JOYPAD_ADDRESS  0xFF00

unsigned char buttons_state = 0xFF;

static bool is_button_pressed(button_t button);
static void set_button_flag(button_t button);
static void clear_button_flag(button_t button);
static bool is_standard_selected(void);
static bool is_direction_selected(void);

void joypad_button_down(button_t button)
{
	bool already_pressed = is_button_pressed(button);
	clear_button_flag(button);

	if(already_pressed)
		return;

	bool is_standard_button = (button > 3) ? true : false;

	if(is_standard_button && is_standard_selected())
		request_interrupt(JOYPAD);
	else if(!is_standard_button && is_direction_selected())
		request_interrupt(JOYPAD);
}

void joypad_button_up(button_t button)
{
	set_button_flag(button);
}

unsigned char get_joypad_state(unsigned char joypad_state)
{
	joypad_state &= 0xF0;

	if(is_standard_selected())
		joypad_state |= buttons_state >> 4;
	else if(is_direction_selected())
		joypad_state |= buttons_state & 0x0F;

	return joypad_state;	/* Possible problem: If the above conditions aren't met, joypad_state
				 * will return 0 for its 4 less signficant bits, indicating that 4
				 * buttons were pressed */ //TODO: Verify in the future
}

//region Helpers

static bool is_button_pressed(button_t button)
{
	return ((buttons_state >> button) & 1) ? false : true;
}

static void set_button_flag(button_t button)
{
	buttons_state |= (1 << button);
}

static void clear_button_flag(button_t button)
{
	buttons_state &= ~(1 << button);
}

static bool is_standard_selected(void)
{
	return ((IO[0] >> 5) & 1) ? false : true;
}

static bool is_direction_selected(void)
{
	return ((IO[0] >> 4) & 1) ? false : true;
}

//endregion