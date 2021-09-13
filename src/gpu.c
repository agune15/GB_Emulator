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

	short tile_x, tile_y, tile_id, tile_addr;

	for (int pixel_num = 0; pixel_num < 160; pixel_num++) {
		//Find tile ID of current pixel tile
		tile_y = (scrollY + current_scanline) / 8;
		if (tile_y > 32)
			tile_y -= 32;

		tile_x += (scrollX + pixel_num) / 8;

		tile_id = (tile_y * 32) + tile_x;

		//Get tile address from ID
		//tile_addr =

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

// LCDC-4: Get BG and Window tile data area address
//TODO: If 0x8000, stored is unsigned (set) or signed (reset)
static unsigned short get_bg_tiledata_addr(void)
{
	if (read_byte(LCDC_ADDRESS >> 4) & 1)
		return 0x8000;
	else
		return 0x8800;
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