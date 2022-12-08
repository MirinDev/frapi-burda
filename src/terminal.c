#include <terminal.h>

#ifdef WIN32
    HANDLE handle;
#endif

void init ()
{
    #ifdef __linux__
        setlocale(LC_ALL, ".UTF8");
        initscr ();
        cbreak ();
        noecho ();
        //keypad (stdscr, G);
        nodelay (stdscr, TRUE);
        timeout (0);
    #endif
    #ifdef WIN32
        handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCP (CP_UTF8);
        SetConsoleOutputCP (CP_UTF8);
    #endif
}
void quit()
{
    #ifdef __linux__
        endwin ();
    #endif
}

#ifdef __linux__
    int kbhit ()
    {
        int ch = getch();

        if (ch != ERR) {
            ungetch (ch);
            return 1;
        } else {
            return 0;
        }
    }
#endif

void setTitle (const char* title)
{
    #ifdef __linux__
        printf ("\033]0;%s\007", title);
    #endif
    #ifdef WIN32
        SetConsoleTitle (title);
    #endif
}

void hide ()
{
    #ifdef WIN32
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo (handle, &info);
        info.bVisible = 0;
        SetConsoleCursorInfo (handle, &info);
    #endif
    #ifdef __linux__
        //printf("\e[?25l");
        curs_set (0);
    #endif
}

void show ()
{
    #ifdef WIN32
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo (handle, &info);
        info.bVisible = 1;
        SetConsoleCursorInfo (handle, &info);
    #endif
    #ifdef __linux__
        //printf("\e[?25h");
        curs_set (1);
    #endif
}

void gotoxy (int x, int y)
{
    #ifdef WIN32
        SetConsoleCursorPosition (handle, (COORD){(short)x, (short)y});
    #endif
    #ifdef __linux__
        //printf ("\x1B[%d;%df", y, x);
        move (y, x);
    #endif
}

void getSize (int* w, int* h)
{
    #ifdef WIN32
        CONSOLE_SCREEN_BUFFER_INFO info;
        GetConsoleScreenBufferInfo (handle, &info);

        *w = (int)(info.srWindow.Right - info.srWindow.Left + 1);
        *h = (int)(info.srWindow.Bottom - info.srWindow.Top + 1);
    #endif
    #ifdef __linux__
        struct winsize size;
        ioctl(0, TIOCGWINSZ, &size);

        *w = size.ws_col;
        *h = size.ws_row;
    #endif
}

void cls ()
{
    #ifdef WIN32
        system ("cls");
    #endif
    #ifdef __linux__
        //printf ("\x1B[2J\x1B[H");
        clear ();
    #endif
}


//-------------------------------------------------------------

Buffer* createBuffer (char clearChar, int width, int height)
{
    Buffer* buffer = (Buffer*)malloc (sizeof(Buffer));

    buffer->pixels = (char*)malloc (sizeof(char)*width*height);
    buffer->w = width;
    buffer->h = height;

    return buffer;
}

void clearBuffer (Buffer* buffer, char clearChar)
{
    memset (buffer->pixels, clearChar, buffer->w*buffer->h);
}

void renderSpriteBS (Buffer* buffer, char blend, int posx, int posy, const char** spr, int width, int height)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x<width; x++)
        {
            if ( (posx + x >= 0) && (posx + x < buffer->w) )
            {
                int pos = (posy + y)*buffer->w+(posx + x);
                char pixel = spr[y][x];
                if ( pos > 0 && pos < buffer->w * buffer->h && pixel != blend)
                    buffer->pixels[pos] = pixel;
            }
        }
    }
}

void renderSprite (Buffer* buffer, int posx, int posy, const char** spr, int width, int height)
{
    renderSpriteBS (buffer, ' ', posx, posy, spr, width, height);
}

void renderBorder (Buffer* buffer, int posx, int posy, int width, int height)
{
    gotoxy (posx - 1, posy-1);
    for (int x = 0; x < width + 2; x++)
    {
        print ("-");
    }
    for (int y = 0; y < height; y++)
    {
        gotoxy (posx - 1, posy+y);
        print ("#");
        gotoxy (posx + width, posy+y);
        print ("#");
    }
    gotoxy (posx - 1, posy + height);
    for (int x = 0; x < width + 2; x++)
    {
        print ("=");
    }
}

//int last = 0;

void renderBuffer (Buffer* buffer, int posx, int posy)
{
    for (int y = 0; y < buffer->h; y++)
    {
        char subBuffer[buffer->w];
        memcpy( subBuffer, &buffer->pixels[y*buffer->w], buffer->w );
        subBuffer[buffer->w] = '\0';

        gotoxy (posx, posy + y);
        print ("%s", subBuffer);

        /*for (int x = 0; x < buffer->w; x++)
        {
            print ("%c", buffer->pixels[y*buffer->w+x]);
        }*/
    }

    //last = 1 - last;

    #ifdef __linux__
        refresh ();
    #endif
}
