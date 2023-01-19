#include <SDL.h>        // SDL stuff
#include <stdio.h>		// printf
#include <string.h>		// strrchr
#include <stdbool.h>	// bool
#include <math.h>		// floor

#include "rom.h"		// load_cartridge
#include "cpu.h"		// init_registers, execute_next_instruction
#include "memory.h"		// init_memory
#include "timer.h"		// update_timer
#include "input.h"		// joypad functions
#include "interrupts.h"	// check_interrupts_state
#include "display.h"	// update_display
#include "gpu.h"		// screen_pixels
#include "cpu_debug.h"  // CPU debugging
#include "gpu_debug.h"  // GPU debugging

// Window related
#define WINDOW_BASE_WIDTH 	160
#define WINDOW_BASE_LENGTH 	144
static SDL_Window *init_SDL_window(void);
static SDL_Renderer *init_SDL_renderer(SDL_Window *window);
static SDL_Texture *init_SDL_texture(SDL_Renderer *renderer);

// Event handling
static void handle_events(void);
static void handle_key_down(SDL_Keysym *keysym);
static void handle_key_up(SDL_Keysym *keysym);

// Program state
static bool close_window = false;

// Gameboy params
#define CYCLES_FRAME 69905
int frame_cycles = CYCLES_FRAME;
int op_cycles = 0;
static void render_frame(SDL_Renderer *renderer, SDL_Texture *texture);

// Update cycle params
unsigned long frame_start;
unsigned long frame_end;
float elapsed_time;

int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;

	if (load_cartridge(argc - 1, *(argv + 1)) != 0) {
		printf("main: ROM couldn't be loaded");
		//return 1;	//Enable when not debugging. If enabled, the console will disappear
                    //TODO: Add debug compilation flag
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

    if ((texture = init_SDL_texture(renderer)) == NULL) {
        printf("main: Unable to initialize texture: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    while (!close_window) {
		// Frame cap
		frame_start = SDL_GetPerformanceCounter();

		handle_events();

		while (frame_cycles > 0) {
            // CPU debugging
            if (is_debugging_CPU) {
                if (!debug_next_instruction) {
                    handle_events();
                    continue;
                }
                store_cpu_state_before_opcode();
            }

            op_cycles = execute_next_instruction();
			frame_cycles -= op_cycles;
			update_timer(op_cycles);
			update_display(op_cycles);
			check_interrupts_state();

            // CPU debugging
            if (is_debugging_CPU) {
                print_cpu_info();
                if (debug_next_instruction)
                    debug_next_instruction = false;
            }
		}

        render_frame(renderer, texture);
		frame_cycles += CYCLES_FRAME;

		// Frame cap
		frame_end = SDL_GetPerformanceCounter();
		elapsed_time = (frame_end - frame_start) / SDL_GetPerformanceFrequency();
		SDL_Delay(floor((16.666f/1.5f) - elapsed_time));
	}

    save_game();

    SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

// Initialize an SDL_Window
static SDL_Window *init_SDL_window(void)
{
	return SDL_CreateWindow("GB Emulator",
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  WINDOW_BASE_WIDTH * 3,
							  WINDOW_BASE_LENGTH * 3,
							  0);
}

// Initialize an SDL_Renderer
static SDL_Renderer *init_SDL_renderer(SDL_Window *window)
{
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	return renderer;
}

// Initialize an SDL_Texture
static SDL_Texture *init_SDL_texture(SDL_Renderer *renderer)
{
    return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                             WINDOW_BASE_WIDTH, WINDOW_BASE_LENGTH);
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
        case SDLK_F1:
            is_debugging_CPU = !is_debugging_CPU;
            break;
        case SDLK_m:
            debug_next_instruction = true;
            break;
        case SDLK_F2:
            print_VRAM_content();
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

// Update renderer with GPU pixel data
static void render_frame(SDL_Renderer *renderer, SDL_Texture *texture)
{
    int texture_pitch = 0;
    void *texture_pixels = NULL;

    if (SDL_LockTexture(texture, NULL, &texture_pixels, &texture_pitch)) {
        printf("main: Unable to lock texture: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    memcpy(texture_pixels, screen_pixels, WINDOW_BASE_WIDTH * WINDOW_BASE_LENGTH * 4);
    SDL_UnlockTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}