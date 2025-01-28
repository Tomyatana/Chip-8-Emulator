#!/bin/sh

./build.sh
gdb -ex "b cpu.c:33" --args chippy "./Testing Roms/test_suite/7-beep.ch8"
