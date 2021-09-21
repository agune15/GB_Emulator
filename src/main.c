#include <SDL2/SDL.h>	// SDL stuff
#include <stdio.h>		// printf
#include <string.h>		// strrchr
#include <stdbool.h>	// bool
#include <math.h>		// floor

#include "rom.h"		// loadROM
#include "cpu.h"		// init_registers, execute_next_instruction
#include "memory.h"		// init_memory
#include "timer.h"		// update_timer
#include "input.h"		// joypad functions
#include "interrupts.h"	// check_interrupts_state
#include "display.h"	// update_display
#include "gpu.h"		// screen_pixels
#include "registers.h"	// registers (DEBUG)

// Window related
#define WINDOW_WIDTH 	160
#define WINDOW_LENGTH 	144
static SDL_Window *init_SDL_window(void);
static SDL_Renderer *init_SDL_renderer(SDL_Window *window);

// Event handling
static void handle_events(void);
static void handle_key_down(SDL_Keysym *keysym);
static void handle_key_up(SDL_Keysym *keysym);

// Program state
static bool close_window = false;

// Emulator related
int frame_cycles = CYCLES_FRAME;
int op_cycles = 0;
static int read_cartridge(int argc, char *path);
static void render_frame(SDL_Renderer *renderer);

// Update related
unsigned long frame_start;
unsigned long frame_end;
float elapsed_time;

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (read_cartridge(argc - 1, *(argv + 1)) != 0) {
		printf("main: ROM couldn't loaded");
		//return 1;	//Enable when not debugging. If enabled, the console will disappear
	}

	init_registers();
	init_memory();
	init_random_seed();

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("main: Unable to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	if ((window = init_SDL_window()) == NULL) {
		printf("main: Unable to initialize window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	if ((renderer = init_SDL_renderer(window)) == NULL) {
		printf("main: Unable to initialize renderer: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	while (!close_window) {
		// Frame cap
		frame_start = SDL_GetPerformanceCounter();

		handle_events();

		while (frame_cycles > 0) {
			op_cycles = execute_next_instruction();
			frame_cycles -= op_cycles;
			update_timer(op_cycles);
			update_display(op_cycles);
			check_interrupts_state();
		}

		render_frame(renderer);
		frame_cycles += CYCLES_FRAME;

		//DEBUG
		//printf("AF: %#x, BC: %#x, DE: %#x, HL: %#x, SP: %#x, PC: %#x \n", registers.AF, registers.BC, registers.DE, registers.HL, registers.SP, registers.PC);
		//printf("PC: %x \n", registers.PC);

		// Frame cap
		frame_end = SDL_GetPerformanceCounter();
		elapsed_time = (frame_end - frame_start) / SDL_GetPerformanceFrequency();
		SDL_Delay(floor(16.666f - elapsed_time));
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

// Initialize an SDL_Window
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

// Initialize an SDL_Renderer
static SDL_Renderer *init_SDL_renderer(SDL_Window *window)
{
	SDL_Renderer *renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	return renderer;
}

//region Event handling

// Handle all SDL events
static void handle_events(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				close_window = true;
				break;
			case SDL_KEYDOWN:
				handle_key_down(&event.key.keysym);
				resume_cpu();
				break;
			case SDL_KEYUP:
				handle_key_up(&event.key.keysym);
				resume_cpu();
				break;
			default:
				break;
		}
	}
}

// Handle all SDL_KEYDOWN events
static void handle_key_down(SDL_Keysym *keysym)
{
	switch (keysym->sym) {
		case SDLK_ESCAPE:
			close_window = true;
			break;
		case SDLK_a:
			joypad_button_down(A);
			break;
		case SDLK_s:
			joypad_button_down(B);
			break;
		case SDLK_BACKSPACE:
			joypad_button_down(SELECT);
			break;
		case SDLK_RETURN:
			joypad_button_down(START);
			break;
		case SDLK_RIGHT:
			joypad_button_down(RIGHT);
			break;
		case SDLK_LEFT:
			joypad_button_down(LEFT);
			break;
		case SDLK_UP:
			joypad_button_down(UP);
			break;
		case SDLK_DOWN:
			joypad_button_down(DOWN);
			break;
		default:
			break;
	}
}

// Handle all SDL_KEYUP events
static void handle_key_up(SDL_Keysym *keysym)
{
	switch (keysym->sym) {
		case SDLK_a:
			joypad_button_up(A);
			break;
		case SDLK_s:
			joypad_button_up(B);
			break;
		case SDLK_BACKSPACE:
			joypad_button_up(SELECT);
			break;
		case SDLK_RETURN:
			joypad_button_up(START);
			break;
		case SDLK_RIGHT:
			joypad_button_up(RIGHT);
			break;
		case SDLK_LEFT:
			joypad_button_up(LEFT);
			break;
		case SDLK_UP:
			joypad_button_up(UP);
			break;
		case SDLK_DOWN:
			joypad_button_up(DOWN);
			break;
		default:
			break;
	}
}

//endregion

//TODO: Function description
//TODO: Can this be moved into rom.c?
static int read_cartridge(int argc, char *path)
{
	if (argc != 1) {
		if(argc < 1)
			printf("main: No file provided\n");
		if(argc > 1)
			printf("main: More than one file was provided\n");
		return 1;
	}

	if (strcmp(strrchr(path, '.'), ".gb") != 0) {
		printf("main: The file provided is not a Game Boy ROM: %s\n", path);
		return 1;
	}

	printf("main: Loading \"%s\"\n", strrchr(path, '\\') + 1);

	return loadROM(path);
}

static void render_frame(SDL_Renderer *renderer)
{
	for (int scanline = 0; scanline < WINDOW_LENGTH; scanline++) {
		for (int pixel = 0; pixel < WINDOW_WIDTH; pixel++) {
			SDL_SetRenderDrawColor(renderer, screen_pixels[scanline][pixel].r,
								   			 screen_pixels[scanline][pixel].g,
								   			 screen_pixels[scanline][pixel].b,
								   			 screen_pixels[scanline][pixel].a);
			SDL_RenderDrawPoint(renderer, pixel, scanline);
		}
	}

	SDL_RenderPresent(renderer);
}
