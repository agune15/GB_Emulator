#include "palette.h"

SDL_Color get_palette_color(color_t color) {
    switch (color) {
        case BLACK:
            return (SDL_Color){44,33,55,255};
        case DARK_GREY:
            return (SDL_Color){68,97,118,255};
        case LIGHT_GREY:
            return (SDL_Color){63,172,149,255};
        case WHITE:
            return (SDL_Color){161,239,140,255};
        default:
            break;
    }
}
