#pragma once

typedef enum {RIGHT, LEFT, UP, DOWN, A, B, SELECT, START} buttons_t;

void joypad_button_down(buttons_t button);
void joypad_button_up(buttons_t button);
unsigned char get_joypad_state(unsigned char joypad_state);