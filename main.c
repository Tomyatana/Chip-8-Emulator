#include "emulator/config.h"
#include "emulator/definitions.h"
#include "emulator/memory.c"
#include "emulator/graphics.c"
#include "emulator/cpu.c"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>


int main(int argc, char* argv[]) {
	if(argc != 2) return 1;
	FILE* exe = fopen(argv[1], "r");

	struct stat st_exe;
	fstat(fileno(exe), &st_exe);

	
	if(fread(RAM+INITIAL_SPACE+1, 1, 0xFFF-INITIAL_SPACE, exe) < 1) return 1;

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

		clock_gettime(CLOCK_MONOTONIC, &current_time);

		long elapsed_timer = (current_time.tv_sec-timer_time.tv_sec) * SECOND_CONST
			+ (current_time.tv_nsec-timer_time.tv_nsec) / 1000;
		long elapsed_cycle = (current_time.tv_sec-cycle_time.tv_sec) * SECOND_CONST
			+ (current_time.tv_nsec-cycle_time.tv_nsec) / 1000;
		
		if(elapsed_timer >= SECOND_CONST/TIMER_FREQ) {
			if(sound_timer > 0)
				sound_timer--;
			if(sound_timer > 0)
				timer--;
			timer_time = current_time;
		}
		if(elapsed_cycle >= SECOND_CONST/FREQ) {
			decode(fetch());
			cycle_time = current_time;
		}

		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT || keyboard[SDL_SCANCODE_ESCAPE])
				running = 0;
		}
	}
	SDL_DestroyWindow(display.window);
	SDL_Quit();
	return 0;
}
