gcc -o build/terminal resources/resources.res Main.c src/terminal.c -Iinclude -Wall -static -static-libgcc
start .\build\terminal.exe
