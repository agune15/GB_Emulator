#include <stdbool.h>
#include "input.h"
#include "interrupts.h"
#include "memory.h"

unsigned char buttons_state = 0xFF;

static bool is_button_pressed(buttons_t button);
static void set_button_flag(buttons_t button);
static void reset_button_flag(buttons_t button);
static bool is_standard_selected(void);
static bool is_direction_selected(void);

// Set button state as pressed
void joypad_button_down(buttons_t button)
{
	bool already_pressed = is_button_pressed(button);
	reset_button_flag(button);

	if(already_pressed)
		return;

	bool is_standard_button = (button > 3) ? true : false;

	if(is_standard_button && is_standard_selected() || !is_standard_button && is_direction_selected())
		request_interrupt(JOYPAD);
}

// Set button state as unpressed
void joypad_button_up(buttons_t button)
{
	set_button_flag(button);
}

// Return joypad state depending on buttons state and selected buttons type
unsigned char get_joypad_state(unsigned char joypad_state)
{
	joypad_state &= 0xF0;

	if(is_standard_selected())
		joypad_state |= buttons_state >> 4;
	else if(is_direction_selected())
		joypad_state |= buttons_state & 0x0F;

	return joypad_state;
}

//region Helpers

// Return the state of the desired button flag
static bool is_button_pressed(buttons_t button)
{
	return ((buttons_state >> button) & 1) ? false : true;
}

// Set button flag in buttons_state
static void set_button_flag(buttons_t button)
{
	buttons_state |= (1 << button);
}

// Reset button flag in buttons_state
static void reset_button_flag(buttons_t button)
{
	buttons_state &= ~(1 << button);
}


// Check if standard buttons are selected
static bool is_standard_selected(void)
{
	return ((IO[0] >> 5) & 1) ? false : true;
}

// Check if direction buttons are selected
static bool is_direction_selected(void)
{
	return ((IO[0] >> 4) & 1) ? false : true;
}

//endregion
