#pragma once

typedef enum {RIGHT, LEFT, UP, DOWN, A, B, SELECT, START} button_t;

void joypad_button_down(button_t button);
void joypad_button_up(button_t button);