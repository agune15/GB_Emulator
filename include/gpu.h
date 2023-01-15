#pragma once

#include <SDL_pixels.h>

#define SCY_ADDRESS  0xFF42		// BG Scroll Y
#define SCX_ADDRESS  0xFF43		// BG Scroll X
#define WY_ADDRESS   0xFF4A		// Window Y position
#define WX_ADDRESS   0xFF4B		// Window X position

#define SPRITE_ATTRIBUTES  0xFE00   // Sprite attributes

extern SDL_Color screen_pixels[144][160];

void draw_scanline(void);
