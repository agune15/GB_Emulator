#pragma once

#include <SDL2/SDL_pixels.h>

#define SCY_ADDRESS  0xFF42		// BG Scroll Y
#define SCX_ADDRESS  0xFF43		// BG Scroll X
#define WY_ADDRESS  0xFF4A		// Window Y position
#define WX_ADDRESS  0xFF4B		// Window X position

extern SDL_Color screen_pixels[144][160];

void draw_scanline(void);
