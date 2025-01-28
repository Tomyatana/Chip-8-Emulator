#ifndef CONFIG_H
#define CONFIG_H

// Window options are 10 times the resolution of the display 
#define DIS_W 640 // Window Width
#define DIS_H 320 // Window Height
#define FREQ 500 // Cycles per second, 500 by default
#define FONT_ADDR 0x50 // Starting font address in ram, 0x50 by convention
#define INITIAL_SPACE 0x1FF // Space to left empty for legacy programs, 0x50 by default
#define RAM_SIZE 0x1000 // Ram Size, 4096b by default

#endif
