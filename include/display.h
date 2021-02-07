#pragma once

#define SCANLINE_CYCLES    456
#define VISIBLE_SCANLINES  144
#define TOTAL_SCANLINES    153

#define LCD_MODE_2_CYCLES  80
#define LCD_MODE_3_CYCLES  172

#define LCDC_ADDRESS  0xFF40	//LCD Control
#define STAT_ADDRESS  0xFF41	//LCD Status
#define LY_ADDRESS    0xFF44	//LCD Current vertical scanline (0 - 153)
#define LYC_ADDRESS   0xFF45	//LCD Compare

#define LY_R_ADDRESS  (LY_ADDRESS - 0xFF00)

void update_display(int cycles);