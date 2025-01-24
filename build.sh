#!/bin/sh

gcc main.c -std=c99 -ggdb -Wall -o chippy $(sdl2-config --cflags --libs) -O1
