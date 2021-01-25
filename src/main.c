/*
 * Main module of the program, responsible for:
 * 	- Displaying a window
 * 	- Handling keyboard inputs
 *
 ****************************************************************************
 *
 * Copyright (C) 2021 Alex Nebot Oller (@agune15)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 *****************************************************************************/

#include <stdbool.h>	// Required for: bool type
#include <SDL2/SDL.h>	// Required for: SDL functions

// Window dimensions
#define WINDOW_WIDTH 	160
#define WINDOW_LENGTH 	144

// Event handling
void handle_events(void);
void handle_key_down(SDL_Keysym *keysym);

// Program state
static bool close_window = false;

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow("GB Emulator",
				       SDL_WINDOWPOS_CENTERED,
				       SDL_WINDOWPOS_CENTERED,
				       WINDOW_WIDTH,
				       WINDOW_LENGTH,
				       0);

	if(window == NULL) {
		SDL_Log("Unable to initialize window: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	while(!close_window)
	{
		handle_events();
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

//region: Event handling

// handle_events: Handle all SDL events
void handle_events(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event))
	{
		switch(event.type) {
			case SDL_QUIT:
				close_window = true;
				break;
			case SDL_KEYDOWN:
				handle_key_down(&event.key.keysym);
				break;
			default:
				break;
		}
	}
}

// handle_key_down: Handle all SDL_KEYDOWN events
void handle_key_down(SDL_Keysym *keysym)
{
	switch(keysym->sym) {
		case SDLK_ESCAPE:
			close_window = true;
			break;
		default:
			break;
	}
}

//endregion