#ifndef INPUT_C
#define INPUT_C

#include "definitions.h"
#include <SDL2/SDL.h>

#define GET_KEY(x) ((key_states>>x)&1)
#define SET_KEY(x) key_states |= (1<<x)

const byte keys[] = {
	0,
	[SDL_SCANCODE_1] = 0x1,
	[SDL_SCANCODE_2] = 0x2,
	[SDL_SCANCODE_3] = 0x3,
	[SDL_SCANCODE_4] = 0xc,
	[SDL_SCANCODE_Q] = 0x4,
	[SDL_SCANCODE_W] = 0x5,
	[SDL_SCANCODE_E] = 0x6,
	[SDL_SCANCODE_R] = 0xd,
	[SDL_SCANCODE_A] = 0x7,
	[SDL_SCANCODE_S] = 0x8,
	[SDL_SCANCODE_D] = 0x9,
	[SDL_SCANCODE_F] = 0xe,
	[SDL_SCANCODE_Z] = 0xa,
	[SDL_SCANCODE_X] = 0x0,
	[SDL_SCANCODE_C] = 0xb,
	[SDL_SCANCODE_V] = 0xf,
};

byte lastKeyPress = 0;
u16 key_states = 0;

#endif
