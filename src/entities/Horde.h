#ifndef HORDE_H_INCLUDED
#define HORDE_H_INCLUDED

#include <SDL.h>

#define HORDE_SIZE 55

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 60
#define HORDE_MOVE_DELTA_INIT 500

#define INVADERS_TEX_INDEX 1

struct Horde
{
    struct Invader
    {
        enum InvaderType
        {
            INVADER_0 = 0,
            INVADER_1 = 1,
            INVADER_2 = 2
        } type;

        int x, y;
        SDL_bool alive;
        SDL_bool hasMoved;
        int deathTimer;
    } array[HORDE_SIZE];

    SDL_bool locked;
    SDL_bool goingRight;

    int moveClip;
    int moveCount;
    int moveTimer;
    int moveTimeout;
};

void Horde_Initialize();
void Horde_Update();
void Horde_Render();

#endif // HORDE_H_INCLUDED