#ifndef GRAPHICS_C
#define GRAPHICS_C

#include "definitions.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RES_W 64
#define RES_H 32

#define SDL_RENDERER_WHITE(renderer) SDL_SetRenderDrawColor((renderer), 255, 255, 255, 255)
#define SDL_RENDERER_BLACK(renderer) SDL_SetRenderDrawColor((renderer), 0, 0, 0, 255)

byte Screen[RES_W*RES_H] = {0};

struct Display {
	SDL_Renderer* renderer;
	SDL_Window* window;
};

struct Display display = {0};

int init_graphics() {
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		goto Error;
	}
	
	SDL_Window* window = SDL_CreateWindow(
		"Chip-8 Emu", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		DIS_W, DIS_H, 0
	);

	if(window == NULL) {
		goto Error;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_RenderSetLogicalSize(renderer, RES_W, RES_H);

	SDL_RENDERER_BLACK(renderer);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);

	display.window = window;
	display.renderer = renderer;

	return 0;

Error:
	fprintf(stderr, "%s\n", SDL_GetError());
	return 1;

}

void update_screen() {
	for(int i = 0; i < RES_W*RES_H; i++) {
		if(Screen[i] > 0) {
			SDL_RENDERER_WHITE(display.renderer);
			SDL_RenderDrawPoint(display.renderer, i%RES_W, i/RES_W);
		}
	}
	SDL_RenderPresent(display.renderer);
}

int draw_point(int x, int y) {
	Screen[RES_W*y+x] = !Screen[RES_W*y+x];
	update_screen();
	return Screen[RES_W*y+x];
}

void clear_screen() {
	memset(Screen, 0, sizeof Screen);
	SDL_RENDERER_BLACK(display.renderer);
	SDL_RenderClear(display.renderer);
	update_screen();
}

#endif
