#include "emulator/config.h"
#include "emulator/definitions.h"
#include "emulator/memory.c"
#include "emulator/graphics.c"
#include "emulator/cpu.c"
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
	if(argc != 2) return 1;
	FILE* exe = fopen(argv[1], "r");

	struct stat st_exe;
	fstat(fileno(exe), &st_exe);

	
	if(fread(RAM+INITIAL_SPACE+1, 1, 0xFFF-INITIAL_SPACE, exe) < 1) return 1;

	init_graphics();
	SDL_Event e;
	byte running = 1;
	while(running) {
		while(SDL_PollEvent(&e) != 0) {
			SDL_Keycode key = e.key.keysym.sym;
			if(e.type == SDL_QUIT || key == SDLK_ESCAPE && e.type == SDL_KEYDOWN)
				running = 0;
		}
		decode(fetch());
	}
	SDL_DestroyWindow(display.window);
	SDL_Quit();
	return 0;
}
