#!/bin/sh
gcc -o build/terminal Main.c src/terminal.c -Iinclude -Wall -lncurses
gnome-terminal -e "./build/terminal"
clear