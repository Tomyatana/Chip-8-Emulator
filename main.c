#include "emulator/config.h"
#include "emulator/definitions.h"
#include "emulator/input.c"
#include "emulator/memory.c"
#include "emulator/graphics.c"
#include "emulator/cpu.c"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

int parseopts(int argc, char* argv[], unsigned long* freq);

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("Expected Chip-8 File\n");
		return 1;
	}
	FILE* exe = fopen(argv[1], "r");
	if(exe == NULL) {
		printf("Expected Chip-8 File\n");
		return 1;
	}
	
	unsigned long frequency = FREQ;

	parseopts(argc, argv, &frequency);

	struct stat st_exe;
	fstat(fileno(exe), &st_exe);

	
	if(fread(RAM+INITIAL_SPACE+1, 1, RAM_SIZE-1-INITIAL_SPACE, exe) < 1) return 1;

	struct timespec timer_time, cycle_time, current_time;
	clock_gettime(CLOCK_MONOTONIC, &timer_time);
	cycle_time = timer_time;

	srand(time(NULL));

	init_graphics();
	SDL_Event e;
	const Uint8* keyboard;
	byte running = 1;
	while(running) {
		keyboard = SDL_GetKeyboardState(NULL);

		// Need to make this better
		if(keyboard[SDL_SCANCODE_1]) SET_KEY(0x1);
		if(keyboard[SDL_SCANCODE_2]) SET_KEY(0x2);
		if(keyboard[SDL_SCANCODE_3]) SET_KEY(0x3);
		if(keyboard[SDL_SCANCODE_4]) SET_KEY(0xc);

		if(keyboard[SDL_SCANCODE_Q]) SET_KEY(0x4);
		if(keyboard[SDL_SCANCODE_W]) SET_KEY(0x5);
		if(keyboard[SDL_SCANCODE_E]) SET_KEY(0x6);
		if(keyboard[SDL_SCANCODE_R]) SET_KEY(0xd);

		if(keyboard[SDL_SCANCODE_A]) SET_KEY(0x7);
		if(keyboard[SDL_SCANCODE_S]) SET_KEY(0x8);
		if(keyboard[SDL_SCANCODE_D]) SET_KEY(0x9);
		if(keyboard[SDL_SCANCODE_F]) SET_KEY(0xe);

		if(keyboard[SDL_SCANCODE_Z]) SET_KEY(0xa);
		if(keyboard[SDL_SCANCODE_X]) SET_KEY(0x0);
		if(keyboard[SDL_SCANCODE_C]) SET_KEY(0xb);
		if(keyboard[SDL_SCANCODE_V]) SET_KEY(0xf);

		clock_gettime(CLOCK_MONOTONIC, &current_time);

		long elapsed_timer = (current_time.tv_sec-timer_time.tv_sec) * SECOND_CONST
			+ (current_time.tv_nsec-timer_time.tv_nsec) / 1000;
		long elapsed_cycle = (current_time.tv_sec-cycle_time.tv_sec) * SECOND_CONST
			+ (current_time.tv_nsec-cycle_time.tv_nsec) / 1000;
		
		if(elapsed_timer >= ((float)SECOND_CONST/TIMER_FREQ)/((double)frequency/FREQ)) {
			if(sound_timer > 0) {
				sound_timer--;
				beep(0);
			} else {
				beep(1); // Unwanted Latency
			}
			if(timer > 0)
				timer--;
			timer_time = current_time;
			if(halted == 1) {
				update_screen();
				halted = 0;
			}
		}
		if(elapsed_cycle >= SECOND_CONST/frequency) {
			cycle_time = current_time;
			if(halted == 0)
				decode(fetch());
			key_states = 0;
		}

		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT || keyboard[SDL_SCANCODE_ESCAPE])
				running = 0;
			if(e.type == SDL_KEYUP && halted > 1) {
				lastKeyPress = keys[e.key.keysym.scancode];
				if(lastKeyPress) {
					V[GET_NIBBLE(halted, 0)] = lastKeyPress;
					halted = 0;
				}
			}
		}
	}
	SDL_DestroyWindow(display.window);
	SDL_Quit();
	return 0;
}

int parseopts(int argc, char* argv[], unsigned long* freq) {
	int option;
	while((option = getopt(argc, argv, "f:")) != -1) {
		switch(option) {
			case 'f':
				*freq = atol(optarg);
				break;
		}
	}
	return 0;
}
