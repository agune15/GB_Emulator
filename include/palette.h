#pragma once

#include <SDL_pixels.h>

typedef enum {BLACK, DARK_GREY, LIGHT_GREY, WHITE} color_t;

SDL_Color get_palette_color(color_t color);
