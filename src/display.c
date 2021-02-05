/*
 *
 */

#include <stdbool.h>
#include "display.h"
#include "memory.h"
#include "interrupts.h"

#define LCDC_R_ADDRESS  (LCDC_ADDRESS - 0xFF00)
#define STAT_R_ADDRESS  (STAT_ADDRESS - 0xFF00)
#define LY_R_ADDRESS    (LY_ADDRESS - 0xFF00)
#define LYC_R_ADDRESS   (LYC_ADDRESS - 0xFF00)

int scanline_counter = SCANLINE_CYCLES;

static bool is_LCD_enabled(void);
static bool is_mode_interrupt_enabled(unsigned char display_status, unsigned char mode);
static void set_LCD_mode(unsigned char *status, unsigned char mode);
static void set_coincidence_flag(unsigned char *status);
static void clear_coincidence_flag(unsigned char *status);
static bool is_coincidence_interrupt_enabled(void);

// Update display with current instruction cycles
void update_display(int cycles)		//TODO: Dissect in smaller functions
{
	unsigned char status = read_byte(STAT_ADDRESS);
	unsigned char *pcurrent_line = &IO[LY_R_ADDRESS];	// This in every update??

	// Update disabled LCD
	if(!is_LCD_enabled()) {
		scanline_counter = SCANLINE_CYCLES;
		*pcurrent_line = 0;
		set_LCD_mode(&status, 1);
		write_byte(STAT_ADDRESS, status);
		return;
	}

	unsigned char last_mode = read_byte(STAT_ADDRESS) & 0x03;
	unsigned char new_mode;

	// Mode update
	if(*pcurrent_line >= VISIBLE_SCANLINES) {
		new_mode = 1;
	}
	else {
		if(scanline_counter >= SCANLINE_CYCLES - LCD_MODE_2_CYCLES) {
			new_mode = 2;
		}
		else if(scanline_counter >= SCANLINE_CYCLES - LCD_MODE_2_CYCLES - LCD_MODE_3_CYCLES)
			new_mode = 3;
		else {
			//if(last_mode == 3)
				//TODO: DrawScanline
			new_mode = 0;
		}
	}
	set_LCD_mode(&status, new_mode);

	// Mode interrupt request
	if(last_mode != new_mode && is_mode_interrupt_enabled(status, new_mode))
		request_interrupt(INTERRUPT_LCD_BIT);

	// Coincidence flag
	if(*pcurrent_line == read_byte(LYC_ADDRESS)) {
		set_coincidence_flag(&status);
		if(is_coincidence_interrupt_enabled())
			request_interrupt(INTERRUPT_LCD_BIT);
	}
	else
		clear_coincidence_flag(&status);

	write_byte(STAT_ADDRESS, status);

	// Scanline update
	scanline_counter -= cycles;

	if(scanline_counter <= 0) {
		(*pcurrent_line)++;
		scanline_counter = SCANLINE_CYCLES;

		if (*pcurrent_line == VISIBLE_SCANLINES)
			request_interrupt(INTERRUPT_VBLANK_BIT);
		if (*pcurrent_line > TOTAL_SCANLINES)
			*pcurrent_line = 0;
	}
}

// Check display status (8th bit of LCD Control)
static bool is_LCD_enabled(void)
{
	return (read_byte(LCDC_ADDRESS) >> 7) & 1;
}

// Check state of desired mode interrupt
static bool is_mode_interrupt_enabled(unsigned char display_status, unsigned char mode)
{
	switch (mode) {
		case 0:
			return ((display_status >> 3) & 1);
		case 1:
			return ((display_status >> 4) & 1);
		case 2:
			return ((display_status >> 5) & 1);
		default:
			return false;
	}
}

// Set LCD mode in *status
static void set_LCD_mode(unsigned char *status, unsigned char mode)
{
	*status = (*status & ~0x03) | (mode & 0x03);
}

// Set coincidence flag in *status
static void set_coincidence_flag(unsigned char *status)
{
	*status |= (1 << 2);
}

// Clear coincidence flag in *status
static void clear_coincidence_flag(unsigned char *status)
{
	*status &= ~(1 << 2);
}

// Check state of coincidence interrupt enabling flag (7th bit of STAT)
static bool is_coincidence_interrupt_enabled(void)
{
	return (read_byte(STAT_ADDRESS) >> 6) & 1;
}