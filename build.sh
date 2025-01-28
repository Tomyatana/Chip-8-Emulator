#!/bin/sh

gcc main.c -lm -std=c99 -ggdb -Wall -o chippy $(sdl2-config --cflags --libs) -O1
