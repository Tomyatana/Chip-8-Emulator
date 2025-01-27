#!/bin/sh

./build.sh
gdb -ex "b cpu.c:167" --args chippy "./Testing Roms/Pong (1 player).ch8"
