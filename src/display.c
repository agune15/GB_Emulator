/*
 *
 */

#include <stdbool.h>
#include "display.h"
#include "memory.h"
#include "interrupts.h"

//TODO: Put it in the header ???
#define LCDC_R_ADDRESS  (LCDC_ADDRESS - 0xFF00)
#define STAT_R_ADDRESS  (STAT_ADDRESS - 0xFF00)
#define LY_R_ADDRESS    (LY_ADDRESS - 0xFF00)
#define LYC_R_ADDRESS   (LYC_ADDRESS - 0xFF00)

int scanline_counter = SCANLINE_CYCLES;

static bool is_LCD_enabled(void);
static bool is_mode_interrupt_enabled(unsigned char display_status, unsigned char mode);

// Update display with current instruction cycles
void update_display(int cycles)
{
	unsigned char status = read_byte(STAT_ADDRESS);
	unsigned char *pcurrent_line = &IO[LY_R_ADDRESS];	// This in every update??

	if(!is_LCD_enabled()) {
		scanline_counter = SCANLINE_CYCLES;
		*pcurrent_line = 0;
		status |= 0x01;
		write_byte(STAT_ADDRESS, status);
		return;
	}

	unsigned char current_mode = read_byte(STAT_ADDRESS) & 0x03;
	unsigned char next_mode;

	if(*pcurrent_line >= VISIBLE_SCANLINES) {
		next_mode = 1;
	}
	else {
		if(scanline_counter >= SCANLINE_CYCLES - LCD_MODE_2_CYCLES) {
			next_mode = 2;
			status
		}
		else if(scanline_counter >= SCANLINE_CYCLES - LCD_MODE_2_CYCLES - LCD_MODE_3_CYCLES)
			next_mode = 3;
		else
			if(current_mode == 3)
				//TODO: DrawScanline
			next_mode = 0;
	}

	//TODO: Mode change interrupt

	//TODO: Coincidence flag

	write_byte(STAT_ADDRESS, status);
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

// Check display status (7th bit of LCD Control)
static bool is_LCD_enabled(void)
{
	return ((read_byte(LCDC_ADDRESS) >> 7) & 1);
}

//
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

//
static void set_LCD_STAT_mode(unsigned char *status, unsigned char mode)
{
	switch (mode) {
		case 0:
			*status |= (1 << 1);
			*status &=
		case 1:
			return ((display_status >> 4) & 1);
		case 2:
			return ((display_status >> 5) & 1);
		case 2:
			return ((display_status >> 5) & 1);
		default:
			return false;
	}




}

