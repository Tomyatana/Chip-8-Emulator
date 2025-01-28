## Chip-8 Emulator
A simple Chip-8 emulator, with the quirks that appear on the Cosmac VIP chip-8 interpreter. 

It uses SDL for the graphics and sound. I haven't tested it in any other machine than mine, so if an error is encountered, please tell me.

### Config.h
---
The file `config.h` file has some definitions which allow you to change parts of the emulator:
+ `DIS_W` & `DIS_H`: Window size and height. (640x320 by default)
+ `FREQ`: The default amount of chip-8 instruction per second. (500 IPS by default)
+ `FONT_ADDR`: The position at which the font information is stored. (0x50 by convention)
+ `INITIAL_SPACE`: For legacy reasons, an space where the roms won't load, this is due to the Cosmac VIP interpreter being located from 0x000 to 0x1FF.
+ `RAM_SIZE`: Amount of ram in the system, 4096 bytes by default, most roms won't need more.
 
For changes in this file to have effect, the program will have to be recompiled.

### Compiling
---
For compiling the program just run `./build.sh`, it will leave a binary named `chippy` in the same directory. You can modify the script, since it's just a one-liner that calls gcc and sdl2-config. The `debug.sh` script was for when i was debugging the game, it's yet another simple two-liner script.

---
#### That's all, feel welcome to fork it or do anything you want with it. ;)