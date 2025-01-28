#ifndef GRAPHICS_C
#define GRAPHICS_C

#include "definitions.h"
#include "config.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define RES_W 64
#define RES_H 32

#define SDL_RENDERER_WHITE(renderer) SDL_SetRenderDrawColor((renderer), 255, 255, 255, 255)
#define SDL_RENDERER_BLACK(renderer) SDL_SetRenderDrawColor((renderer), 0, 0, 0, 255)

byte Screen[RES_W*RES_H] = {0};
SDL_AudioDeviceID audio_device;

struct Display {
	SDL_Renderer* renderer;
	SDL_Window* window;
};

struct Display display = {0};

void beep_callback(void* userdata, Uint8* stream, int len) {
	Sint16* buffer = (Sint16*) stream;
	static double phase = 0.0;
	double phase_inc = 2.0 * 3.1415926535 * 8000 / 4096;

	for(int i = 0; i < len / 2; i++) {
		buffer[i] = (Sint16)(28000 * sin(phase));
		phase += phase_inc;
		if(phase >= 2.0 * 3.1415926535) {
			phase -= 2.0 * 3.1415926535;
		}
	}
}

int init_graphics() {
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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

	SDL_AudioSpec spec;
	spec.freq = 8000;
	spec.format = AUDIO_S16SYS;
	spec.channels = 1;
	spec.samples = 4096;
	spec.callback = beep_callback;
	audio_device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);

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
		} else {
			SDL_RENDERER_BLACK(display.renderer);
			SDL_RenderDrawPoint(display.renderer, i%RES_W, i/RES_W);
		}
	}
	SDL_RenderPresent(display.renderer);
}

int draw_point(int x, int y) {
	Screen[RES_W*y+x] = !Screen[RES_W*y+x];
	return Screen[RES_W*y+x];
}

void clear_screen() {
	memset(Screen, 0, RES_W * RES_H);
	SDL_RENDERER_BLACK(display.renderer);
	SDL_RenderClear(display.renderer);
	SDL_RenderPresent(display.renderer);
}

void beep(int n) {
	SDL_PauseAudioDevice(audio_device, n);
	return;
}

#endif
