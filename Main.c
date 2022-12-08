#include <terminal.h>
#include <sprites.h>

#define WIDTH 60
#define HEIGHT 20
#define OFFSETX 9
#define OFFSETY 2

//clound
float cloundX[6];
float cloundY[6];
int cloundId[6];
int cloundSpd[6];
const int cloundsSize = sizeof (cloundX) / sizeof (*cloundX);

//pipe
const float pipeVel = 8.0f;
float pipeX[3];
int pipeY[3];
int pipeUsed[3];
const int pipeSize = sizeof (pipeX) / sizeof (*pipeX);

//bird
const int posX = 4;
float posY = (float)(HEIGHT/2-birdH/2);

float vel = 0.0f;
const float gvdForce = 12.0f;
const float maxGvd = 8.0f;
const float jumpForce = 8.0f;

//animation
const float maxFrames = 2;
const float animFps = 0.5f;
int frame = 0;
float frameTime = 0.0f;

//points
int points = 0;
int Bestpoints = 0;

//simple engine variables
clock_t lastTime;
Buffer* buffer;
int lWindowW, lWindowH;
int windowW, windowH;

void load ()
{
    //restart variables
    points = 0;
    frame = 0;
    frameTime = 0.0f;
    posY = (float)(HEIGHT/2-birdH/2);
    vel = 0.0f;
    
    //load clounds
    srand (time(NULL));
    for (int i = 0; i < cloundsSize; i++)
    {
        cloundX[i] = rand ()%WIDTH;
        cloundY[i] = rand ()%8;
        cloundId[i] = rand ()%3+1;
        cloundSpd[i] = rand ()%8 + 6;
    }

    //load pipes
    srand (time(NULL));
    for (int i = 0; i < pipeSize; i++)
    {
        int d = (rand()%3 + 4) * 4;

        if ( i == 0)
            pipeX[i] = WIDTH + d;
        else
            pipeX[i] = pipeX[i-1] + d;

        if ( d == 20 )
            pipeY[i] = rand()%5-4;
        else
            pipeY[i] = rand()%9-8;
        
        pipeUsed[i] = 0;
    }

    //simple engine variables
    getSize (&windowW, &windowH);
    lWindowW = windowW;lWindowH = windowH;
}

void unload ()
{
    //simple engine variables
    free (buffer);
}

void resizeWidnow ()
{
    hide ();
    cls ();
    
    renderBorder (buffer, OFFSETX, OFFSETY, WIDTH, HEIGHT);
    gotoxy (OFFSETX+WIDTH/2-11, 0);
    print ("frapi bird terminal 2.0");
}

void update ()
{
    //simple engine variables
    clock_t timer = clock ();
    float dt = (float)(timer - lastTime) / CLOCKS_PER_SEC;
    lastTime = timer;

    //gotoxy (0, 0);
    //print ("fps: %d          ", (int)(1.0f/dt) + 1);

    getSize (&windowW, &windowH);
    if (windowW != lWindowW || windowH != lWindowH)
    {
        resizeWidnow ();
    }
    lWindowW = windowW;lWindowH = windowH;

    //bird animation
    if (vel < 0.0f){
        frameTime += dt;
        if (frameTime > animFps)
        {
            frameTime = 0.0f;
            frame ++;
            if ( frame > maxFrames - 1 )
                frame = 0;
        }
    }else
        frame = 0;
    //bird phisics
    if (vel < maxGvd)
        vel += gvdForce * dt;
    posY += vel * dt;

    if ((int)posY < 0 || (int)posY + birdH > HEIGHT)
        load ();

    //keys events
    char ch = '\0';
    while (kbhit ())
    {
        ch = getch ();
        if (ch == ' ')
        {
            frame = 1;
            vel = -jumpForce;
        }
    }

    //clounds update
    for (int i = 0; i < cloundsSize; i++)
    {
        cloundX[i] -= (float)cloundSpd[i] * dt;
        int reloop = 0;

        if (cloundId[i] == 1 && (int)cloundX[i] + cloudW1 < 0)
            reloop = 1;
        if (cloundId[i] == 2 && (int)cloundX[i] + cloudW2 < 0)
            reloop = 1;
        if (cloundId[i] == 3 && (int)cloundX[i] + cloudW3 < 0)
            reloop = 1;

        if (reloop)
        {
            cloundX[i] = (float)WIDTH;
            cloundY[i] = rand ()%8;
            cloundId[i] = rand ()%3+1;
            cloundSpd[i] = rand ()%8 + 6;
        }
    }

    //pipes update
    for (int i = 0; i < pipeSize; i++)
    {
        pipeX[i] -= pipeVel * dt;

        if ((posX < pipeX[i] + pipeW && posX + birdW > pipeX[i]) &&
            (
                ((int)posY + 1 < pipeY[i] + 13 && (int)posY + birdH > pipeY[i]) ||
                ((int)posY + 1 < pipeY[i] + pipeH && (int)posY + birdH > pipeY[i] + 18)
            ))
        {
            if (points > Bestpoints)
                Bestpoints = points;
            load ();
        }
        else if ( !pipeUsed[i] && (int)pipeX[i] + pipeW / 2 == posX )
        {
            points++;
            pipeUsed[i] = 1;
        }

        if ((int)pipeX[i] + pipeW < 0)
        {
            int d = (rand()%3 + 5) * 4;

            pipeX[i] = WIDTH + d;
            if ( d == 20 )
                pipeY[i] = rand()%5-4;
            else
                pipeY[i] = rand()%9-8;

            pipeUsed[i] = 0;
        }
    }

}

void render ()
{
    clearBuffer (buffer, ' ');
    //clearBufferBackground (buffer, background);
    renderSprite (buffer, 0, 0, background, backgroundW, backgroundH);

    //render clounds
    for (int i = 0; i < cloundsSize; i++)
    {
        if (cloundId[i] == 1)
            renderSpriteBS (buffer, '.', (int)cloundX[i],  (int)cloundY[i], cloudSpr1, cloudW1, cloundH1);
        if (cloundId[i] == 2)
            renderSpriteBS (buffer, '.', (int)cloundX[i],  (int)cloundY[i], cloudSpr2, cloudW2, cloundH2);
        if (cloundId[i] == 3)
            renderSpriteBS (buffer, '.', (int)cloundX[i],  (int)cloundY[i], cloudSpr3, cloudW3, cloundH3);
    }
    
    //render bird
    renderSprite (buffer, posX, posY, birdSpr[frame], birdW, birdH);

    //rend pipes
    for (int i = 0; i < pipeSize; i++)
        renderSpriteBS (buffer, '.', pipeX[i], pipeY[i], pipeSpr, pipeW, pipeH);

    //render buffer
    renderBuffer (buffer, OFFSETX, OFFSETY);

    //print points
    //gotoxy (0, 0);

    char log[WIDTH] = { 0 };
    sprintf (log, "points: %d // best points: %d", points, Bestpoints);
    
    gotoxy ( OFFSETX + WIDTH / 2 - strlen (log)/2, OFFSETY + HEIGHT + 1);
    print ("%s", log);

}

int main (int argc, char* argv[])
{
    init ();
    resizeWidnow ();

    setTitle ("frapi burda");
    buffer = createBuffer (' ', WIDTH, HEIGHT);
    load ();

    while (1)
    {
        update ();
        render ();

        setTitle ("frapi burda");
    }

    unload ();

    show ();
    quit ();

    return 0;
}