/*
 *
 */

#include <stdbool.h>
#include "display.h"
#include "memory.h"
#include "interrupts.h"

//TODO: Make them static?
int cycles_counter = SCANLINE_CYCLES;
unsigned char status;
unsigned char *pcurrent_line;

static void update_globals(void);
static void update_LCD_disabled(void);
static void update_LCD_mode(void);
static void update_coincidence_flag(void);
static void update_scanline(int cycles);

// Helpers
static bool is_LCD_enabled(void);
static bool is_mode_interrupt_enabled(unsigned char mode);
static void set_LCD_mode(unsigned char mode);
static void set_coincidence_flag(void);
static void clear_coincidence_flag(void);
static bool is_coincidence_interrupt_enabled(void);

// Update display with current instruction cycles
void update_display(int cycles)
{
	update_globals();

	if(!is_LCD_enabled()) {
		update_LCD_disabled();
		return;
	}

	update_LCD_mode();
	update_coincidence_flag();

	write_byte(STAT_ADDRESS, status);

	update_scanline(cycles);
}

// Update global variables
void update_globals(void)
{
	status = read_byte(STAT_ADDRESS);
	pcurrent_line = &IO[LY_R_ADDRESS];
}

// Update disabled display
static void update_LCD_disabled(void)
{
	cycles_counter = SCANLINE_CYCLES;
	*pcurrent_line = 0;
	set_LCD_mode(1);
	write_byte(STAT_ADDRESS, status);
}

// Update display mode
static void update_LCD_mode(void)
{
	unsigned char last_mode = status & 0x03;
	unsigned char new_mode;

	if(*pcurrent_line >= VISIBLE_SCANLINES) {
		new_mode = 1;
	}
	else {
		if(cycles_counter >= SCANLINE_CYCLES - LCD_MODE_2_CYCLES) {
			new_mode = 2;
		}
		else if(cycles_counter >= SCANLINE_CYCLES - LCD_MODE_2_CYCLES - LCD_MODE_3_CYCLES)
			new_mode = 3;
		else {
			//if(last_mode == 3)
				//TODO: DrawScanline
			new_mode = 0;
		}
	}
	set_LCD_mode(new_mode);

	if(last_mode != new_mode && is_mode_interrupt_enabled(new_mode))
		request_interrupt(LCD);
}

// Update display's coincidence flag
void update_coincidence_flag(void)
{
	if(*pcurrent_line == read_byte(LYC_ADDRESS)) {
		set_coincidence_flag();
		if(is_coincidence_interrupt_enabled())
			request_interrupt(LCD);
	}
	else
		clear_coincidence_flag();
}

// Update display's scanline with given cycles
static void update_scanline(int cycles)
{
	cycles_counter -= cycles;

	if(cycles_counter <= 0) {
		(*pcurrent_line)++;
		cycles_counter = SCANLINE_CYCLES;

		if (*pcurrent_line == VISIBLE_SCANLINES)
			request_interrupt(VBLANK);
		if (*pcurrent_line > TOTAL_SCANLINES)
			*pcurrent_line = 0;
	}
}

//region Helpers

// Check display status (8th bit of LCD Control)
static bool is_LCD_enabled(void)
{
	return (read_byte(LCDC_ADDRESS) >> 7) & 1;
}

// Check state of desired mode interrupt
static bool is_mode_interrupt_enabled(unsigned char mode)
{
	switch (mode) {
		case 0:
			return ((status >> 3) & 1);
		case 1:
			return ((status >> 4) & 1);
		case 2:
			return ((status >> 5) & 1);
		default:
			return false;
	}
}

// Set LCD mode in status
static void set_LCD_mode(unsigned char mode)
{
	status = (status & ~0x03) | (mode & 0x03);
}

// Set coincidence flag in status
static void set_coincidence_flag(void)
{
	status |= (1 << 2);
}

// Clear coincidence flag in status
static void clear_coincidence_flag(void)
{
	status &= ~(1 << 2);
}

// Check state of coincidence interrupt enabling flag (7th bit of STAT)
static bool is_coincidence_interrupt_enabled(void)
{
	return (read_byte(STAT_ADDRESS) >> 6) & 1;
}

//endregion