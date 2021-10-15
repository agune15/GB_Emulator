#include <stdbool.h>
#include <stdio.h>
#include "gpu.h"
#include "memory.h"
#include "display.h"

SDL_Color screen_pixels[144][160];

static void render_tiles(void);

// Helpers
static bool is_tile_display_enabled(void);
static bool is_sprite_display_enabled(void);
static unsigned short get_bg_tilemap_addr(void);
static unsigned short get_bg_tiledata_addr(void);
static bool is_window_enabled(void);
static bool get_window_tilemap_addr(void);

// Draw current scanline pixels on screen
void draw_scanline(void)
{
	if (is_tile_display_enabled())
		render_tiles();

	//if (is_sprite_display_enabled())
		//Render sprites
}

//TODO: Description
//TODO: Split function in many smaller ones
static void render_tiles(void)
{
	unsigned char pixel_relative_addr, pixel_lsB, pixel_msB, pixel_color_id, pixel_palette_id, pixel_col, pixel_row, pixel_bit_num;
	SDL_Color pixel_color;
	unsigned short tile_col, tile_row, tile_map_addr, tile_data_addr;
	short tile_relative_pos;

	unsigned char current_scanline = read_byte(LY_ADDRESS);
	unsigned char scrollY = read_byte(SCY_ADDRESS);
	unsigned char scrollX = read_byte(SCX_ADDRESS);
	unsigned char windowY = read_byte(WY_ADDRESS);
	unsigned char windowX = read_byte(WX_ADDRESS) - 7;

	bool is_using_window = false;
	if (is_window_enabled() && windowY <= current_scanline)
		is_using_window = true;

	unsigned short map_addr = (is_using_window) ? get_window_tilemap_addr() : get_bg_tilemap_addr();
	unsigned short data_addr = get_bg_tiledata_addr();
	bool unsign = (data_addr == 0x8000) ? true : false; //TODO: change to something more clear, like unsigned_tile_data_pos
	//bool unsign = (map_addr == 0x9C00) ? true : false;

	unsigned char bg_palette = read_byte(0xFF47);

	// Find row of current pixel
	pixel_row = (is_using_window) ? current_scanline - windowY : (scrollY + current_scanline);

	// Find row of current tile
	tile_row = pixel_row / 8;

	//TODO: Needed?
	//if (tile_row > 32)
	//	tile_row -= 32;

	for (int pixel_num = 0; pixel_num < 160; pixel_num++) {
		// Find column of current pixel
		pixel_col = scrollX + pixel_num;
		if (is_using_window && pixel_num >= windowX)	//TODO: Why?
			pixel_col -= windowX;

		// Find column of current tile
		tile_col = pixel_col / 8;

		// Get tile address in tile map from position
		tile_map_addr = map_addr + (tile_row * 32) + tile_col;

		// Get tile data relative position from tile address
		tile_relative_pos = (unsign) ? read_byte(tile_map_addr) : (signed char) read_byte(tile_map_addr);

		// Get tile data address from tile relative position
		tile_data_addr = data_addr + (tile_relative_pos * 16);

		// Get pixel relative addr
		pixel_relative_addr = (pixel_row % 8) * 2;

		// Get pixel data
		pixel_lsB = read_byte(tile_data_addr + pixel_relative_addr);
		pixel_msB = read_byte(tile_data_addr + pixel_relative_addr + 1);

		// Get pixel color ID
		pixel_bit_num = 7 - (pixel_col % 8);
		pixel_color_id = (pixel_lsB >> pixel_bit_num) & 1;
		pixel_color_id |= ((pixel_msB >> pixel_bit_num) & 1) << 1;

		// Get pixel palette ID from color ID
		switch (pixel_color_id) {
			case 0:
				pixel_palette_id = bg_palette & 3;
				break;
			case 1:
				pixel_palette_id = (bg_palette >> 2) & 3;
				break;
			case 2:
				pixel_palette_id = (bg_palette >> 4) & 3;
				break;
			case 3:
				pixel_palette_id = (bg_palette >> 6) & 3;
				break;
			default:
				break;
		}

		// Get RGB from palette ID
		switch (pixel_palette_id) {
			case 0:
				pixel_color = (SDL_Color){155, 188, 15, 255};
				break;
			case 1:
				pixel_color = (SDL_Color){139, 172, 15, 255};
				break;
			case 2:
				pixel_color = (SDL_Color){48, 98, 48, 255};
				break;
			case 3:
				pixel_color = (SDL_Color){15, 56, 15, 255};
				break;
			default:
				break;
		}

		// Add pixel RGB to screen pixels array
		screen_pixels[current_scanline][pixel_num] = pixel_color;
	}
}

//region Helpers

// LCDC-0: Check if tiles need to be displayed
static bool is_tile_display_enabled(void)
{
	return read_byte(LCDC_ADDRESS) & 1;
}

// LCDC-1: Check if sprites need to be displayed
static bool is_sprite_display_enabled(void)
{
	return (read_byte(LCDC_ADDRESS) >> 1) & 1;
}

// LCDC-3: Get BG tile map area address
static unsigned short get_bg_tilemap_addr(void)
{
	if (read_byte(LCDC_ADDRESS >> 3) & 1)
		return 0x9C00;
	else
		return 0x9800;
}

// LCDC-4: Get BG and Window tile data base address
//TODO: If 0x8000, stored is unsigned (set) or signed (reset)
static unsigned short get_bg_tiledata_addr(void)
{
	if (read_byte(LCDC_ADDRESS >> 4) & 1)
		return 0x8000;
	else
		return 0x9000;
}

// LCDC-5: Check if Window is enabled
static bool is_window_enabled(void)
{
	return (read_byte(LCDC_ADDRESS) >> 5) & 1;
}

// LCDC-6: Get Window tile map area address
static bool get_window_tilemap_addr(void)
{
	if (read_byte(LCDC_ADDRESS >> 6) & 1)
		return 0x9C00;
	else
		return 0x9800;
}

//endregion