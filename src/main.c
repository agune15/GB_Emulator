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

#include <SDL2/SDL.h>	// SDL stuff
#include <stdio.h>	// printf
#include <string.h>	// strrchr
#include <stdbool.h>
//#include <stdint.h>

#include "rom.h"	// loadROM
#include "cpu.h"	// init_registers
#include "memory.h"	// init_memory
#include "timer.h"	// update_timer

// Window related
#define WINDOW_WIDTH 	160
#define WINDOW_LENGTH 	144
static SDL_Window *init_SDL_window(void);

// Event handling
static void handle_events(void);
static void handle_key_down(SDL_Keysym *keysym);

// Program state
static bool close_window = false;

// Emulator related
static int read_cartridge(int argc, char *path);

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if(read_cartridge(argc - 1, *(argv + 1)) != 0) {
		printf("main: ROM couldn't loaded");
		//return 1;	//Enable when not debugging. If enabled, the console will disappear
	}

	init_registers();
	init_memory();

	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("main: Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	if((window = init_SDL_window()) == NULL) {
		printf("main: Unable to initialize window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	while(!close_window) {
		handle_events();

		//TODO: udpate_timer(instruction_cycles)
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

static SDL_Window *init_SDL_window(void)
{
	SDL_Window *window = NULL;

	window = SDL_CreateWindow("GB Emulator",
				  SDL_WINDOWPOS_CENTERED,
				  SDL_WINDOWPOS_CENTERED,
				  WINDOW_WIDTH,
				  WINDOW_LENGTH,
				  0);

	return window;
}

//region Event handling

// handle_events: Handle all SDL events
static void handle_events(void)
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
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
static void handle_key_down(SDL_Keysym *keysym)
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

static int read_cartridge(int argc, char *path)
{
	if(argc != 1) {
		if(argc < 1)
			printf("main: No file provided\n");
		if(argc > 1)
			printf("main: More than one file was provided\n");
		return 1;
	}

	if(strcmp(strrchr(path, '.'), ".gb") != 0) {
		printf("main: The file provided is not a Game Boy ROM: %s\n", path);
		return 1;
	}

	printf("main: Loading \"%s\"\n", strrchr(path, '\\') + 1);

	return loadROM(path);
}