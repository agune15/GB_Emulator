#include <stdbool.h>
#include "gpu.h"
#include "memory.h"
#include "display.h"

static void render_tiles(void);

// Helpers
static bool is_sprite_display_enabled(void);
static bool is_tile_display_enabled(void);
static unsigned short get_bg_tiledata_addr(void);
static unsigned short get_bg_tilemap_addr(void);

void draw_scanline(void)
{
	//if (is_sprite_display_enabled())
		//Render sprites

	if (is_tile_display_enabled())	//TODO: Maybe this should go before render_sprites
		render_tiles();
}

static void render_tiles(void)
{
	unsigned char current_scanline = read_byte(LY_ADDRESS);
	unsigned char scrollY = read_byte(SCY_ADDRESS);
	unsigned char scrollX = read_byte(SCX_ADDRESS);

	unsigned short tile_col, tile_row, tile_map_addr, tile_data_addr;
	short tile_relative_pos;

	unsigned short bg_map_addr = get_bg_tilemap_addr();
	unsigned short bg_data_addr = get_bg_tiledata_addr();
	bool unsign = (bg_data_addr == 0x8000) ? true : false; //TODO: change to something more clear, like unsigned_tile_data_pos

	unsigned char pixel_relative_addr, pixel_lsB, pixel_msB, pixel_color_id, pixel_palette_id;

	for (int pixel_num = 0; pixel_num < 160; pixel_num++) {
		// Find position of current tile
		tile_row = (scrollY + current_scanline) / 8;
		if (tile_row > 32)
			tile_row -= 32;

		tile_col += (scrollX + pixel_num) / 8;

		// Get tile address in tile map from position
		tile_map_addr = bg_map_addr + (tile_row * 32) + tile_col;

		// Get tile data relative position from tile address
		tile_relative_pos = (unsign) ? read_byte(tile_map_addr) : (signed char) read_byte(tile_map_addr);

		// Get tile data address from tile relative position
		tile_data_addr = bg_data_addr + tile_relative_pos * 16;

		// Get pixel relative addr
		pixel_relative_addr = (tile_row % 8) * 2;

		// Get pixel data
		pixel_lsB = read_byte(tile_data_addr + pixel_relative_addr);
		pixel_msB = read_byte(tile_data_addr + pixel_relative_addr + 1);

		// Get pixel color id
		pixel_color_id = (pixel_lsB >> (tile_col % 8)) & 1;
		pixel_color_id |= ((pixel_msB >> (tile_col % 8)) & 1) << 1;
	}
}

//region Helpers

// LCDC-1: Check if sprites need to be displayed
static bool is_sprite_display_enabled(void)
{
	return (read_byte(LCDC_ADDRESS) >> 1) & 1;
}

// LCDC-0: Check if tiles need to be displayed
static bool is_tile_display_enabled(void)
{
	return read_byte(LCDC_ADDRESS) & 1;
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

// LCDC-3: Get BG tile map area address
static unsigned short get_bg_tilemap_addr(void)
{
	if (read_byte(LCDC_ADDRESS >> 3) & 1)
		return 0x9800;
	else
		return 0x9C00;
}



//endregion