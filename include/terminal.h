#ifndef TERMINAL_H_
#define TERMINAL_H_

#ifdef WIN32
    #include <windows.h>
    #include <conio.h>
    #define print printf
#endif
#ifdef __linux__
    #include <ncurses.h>
    #include <sys/ioctl.h>
    #define print printw
    int kbhit ();
#endif

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void hide ();
void show ();
void cls ();

void setTitle (const char* title);
void gotoxy (int x, int y);
void getSize (int* w, int* h);

void init ();
void quit ();

typedef struct Buffer_t{
    char* pixels;
    int w, h;
} Buffer;

Buffer* createBuffer (char clearChar, int width, int height);
void clearBuffer (Buffer* buffer, char clearChar);
void renderSprite (Buffer* buffer, int posx, int posy, const char** spr, int width, int height);
void renderSpriteBS (Buffer* buffer, char blend, int posx, int posy, const char** spr, int width, int height);
void renderBuffer (Buffer* buffer, int posx, int posy);
void renderBorder (Buffer* buffer, int posx, int posy, int width, int height);

#endif