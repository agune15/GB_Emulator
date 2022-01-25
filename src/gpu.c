#include <stdbool.h>
#include <stdio.h>
#include "gpu.h"
#include "memory.h"
#include "display.h"
#include "palette.h"

SDL_Color screen_pixels[VISIBLE_SCANLINES][160];

static void render_tiles(void);
static void render_sprites(void);

// Helpers
static bool is_tile_display_enabled(void);
static bool is_sprite_display_enabled(void);
static bool is_using_8x16_sprites(void);
static unsigned short get_bg_tilemap_addr(void);
static unsigned short get_bg_tiledata_addr(void);
static bool is_window_enabled(void);
static unsigned short get_window_tilemap_addr(void);

// Draw current scanline pixels on screen
void draw_scanline(void)
{
	if (is_tile_display_enabled())
		render_tiles();

	if (is_sprite_display_enabled())
		render_sprites();
}

// Render BG and Window tiles
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

	unsigned char bg_palette = read_byte(0xFF47);

	// Find row of current pixel
	pixel_row = (is_using_window) ? current_scanline - windowY : scrollY + current_scanline;

	// Find row of current tile
	tile_row = pixel_row / 8;

	for (int pixel_num = 0; pixel_num < 160; pixel_num++) {
		// Find column of current pixel
		pixel_col = scrollX + pixel_num;
		if (is_using_window && pixel_num >= windowX)
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
				pixel_color = get_palette_color(WHITE);
				break;
			case 1:
				pixel_color = get_palette_color(LIGHT_GREY);
				break;
			case 2:
				pixel_color = get_palette_color(DARK_GREY);
				break;
			case 3:
				pixel_color = get_palette_color(BLACK);
				break;
			default:
				break;
		}

		// Add pixel RGB to screen pixels array
		screen_pixels[current_scanline][pixel_num] = pixel_color;
	}
}

// Render sprites
//TODO: Split in many smaller functions
// Iterates through every sprite in every scanline, not very efficient. Could maybe be executed everytime scanline 144 is reached
static void render_sprites(void)
{
	unsigned char sprite_table_index, sprite_col, sprite_row, sprite_relative_pos, sprite_attribs, sprite_height, sprite_palette;
	unsigned char pixel_row, pixel_lsB, pixel_msB, pixel_color_id, pixel_palette_id, pixel_screen_X;
	unsigned short sprite_table_addr, pixel_data_addr, pixel_palette_addr;
	bool sprite_Y_flipped, sprite_X_flipped;
	SDL_Color pixel_color;

	unsigned char current_scanline = read_byte(LY_ADDRESS);
	bool using_8x16_sprites = is_using_8x16_sprites();
	sprite_height = (using_8x16_sprites) ? 16 : 8;

	// Loop through sprites
	for (int sprite_num = 0; sprite_num < 40; sprite_num++) {
		// Get sprite index in the sprite attributes table
		sprite_table_index = sprite_num * 4;

		// Get sprite table address of sprite
		sprite_table_addr = SPRITE_ATTRIBUTES + sprite_table_index;

		// Get sprite data from the attributes table
		sprite_row = read_byte(sprite_table_addr) - 16;
		sprite_col = read_byte(sprite_table_addr + 1) - 8;
		sprite_relative_pos = read_byte(sprite_table_addr + 2);
		sprite_attribs = read_byte(sprite_table_addr + 3);

		// Check if sprite is flipped vertically or horizontally
		sprite_Y_flipped = (sprite_attribs >> 6) & 1;
		sprite_X_flipped = (sprite_attribs >> 5) & 1;

		// Check if scanline intercepts with sprite
		if (current_scanline >= sprite_row && current_scanline <= (sprite_row + sprite_height-1)) {
			// Pixel row to render
			pixel_row = current_scanline - sprite_row;

			if (sprite_Y_flipped)
				pixel_row = sprite_height - pixel_row;

			// Pixel data
			pixel_data_addr = 0x8000 + (sprite_relative_pos * 16) + (pixel_row * 2);
			pixel_lsB = read_byte(pixel_data_addr);
			pixel_msB = read_byte(pixel_data_addr + 1);

			// Loop through pixel row to render
			for (int pixel_num = 0; pixel_num <= 7; pixel_num++) {
				int pixel_bit_num = 7 - pixel_num;

				// Read pixel bits backwards if sprite is X flipped
				if (sprite_X_flipped)
					pixel_bit_num = 7 - pixel_bit_num;

				// Get pixel color ID
				pixel_color_id = (pixel_lsB >> pixel_bit_num) & 1;
				pixel_color_id |= ((pixel_msB >> pixel_bit_num) & 1) << 1;

				// Get pixel palette
				pixel_palette_addr = ((sprite_attribs >> 4) & 1) ? 0xFF49 : 0xFF48;
				sprite_palette = read_byte(pixel_palette_addr);

				// Get pixel palette ID from color ID
				switch (pixel_color_id) {
					case 0:
						pixel_palette_id = sprite_palette & 3;
						break;
					case 1:
						pixel_palette_id = (sprite_palette >> 2) & 3;
						break;
					case 2:
						pixel_palette_id = (sprite_palette >> 4) & 3;
						break;
					case 3:
						pixel_palette_id = (sprite_palette >> 6) & 3;
						break;
					default:
						break;
				}

				// White is transparent for sprites
				if (pixel_palette_id == 0)
					continue;

				// Get RGB from palette ID
				switch (pixel_palette_id) {
					case 1:
						pixel_color = get_palette_color(LIGHT_GREY);
						break;
					case 2:
						pixel_color = get_palette_color(DARK_GREY);
						break;
					case 3:
						pixel_color = get_palette_color(BLACK);
						break;
					default:
						break;
				}

				// Get pixel screen X pos
				pixel_screen_X = sprite_col + pixel_num;

				// Check if pixel is out of screen bounds
				if (current_scanline < 0 || current_scanline > 143 || pixel_screen_X < 0 || pixel_screen_X > 159)
					continue;

				// Check if sprite should be rendered behind BG
				if (sprite_attribs >> 7 & 1) {
					// Check if current pixel is not white
					if (screen_pixels[current_scanline][pixel_screen_X].r != 161)
						continue;
				}

				// Add pixel RGB to screen pixels array
				screen_pixels[current_scanline][pixel_screen_X] = pixel_color;
			}
		}
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

// LCDC-2: Check if the game is using 8x16 sprites
static bool is_using_8x16_sprites(void)
{
	return (read_byte(LCDC_ADDRESS) >> 2) & 1;
}

// LCDC-3: Get BG tile map area address
static unsigned short get_bg_tilemap_addr(void)
{
	if ((read_byte(LCDC_ADDRESS) >> 3) & 1)
		return 0x9C00;
	else
		return 0x9800;
}

// LCDC-4: Get BG and Window tile data base address
//TODO: If 0x8000, stored is unsigned (set) or signed (reset)
static unsigned short get_bg_tiledata_addr(void)
{
	if ((read_byte(LCDC_ADDRESS) >> 4) & 1)
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
static unsigned short get_window_tilemap_addr(void)
{
	if ((read_byte(LCDC_ADDRESS) >> 6) & 1)
		return 0x9C00;
	else
		return 0x9800;
}

//endregion