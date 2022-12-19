#include "../App.h"
#include <stdio.h>

void Cannon_Initialize()
{
    struct Cannon* cannon = &App_Get()->game.cannon;
    cannon->x = CANNON_X_INIT;
    cannon->lives = CANNON_LIVES_TOTAL;
    cannon->moveTimer = 0;
    cannon->deathTimer = 0;
    cannon->shotTimer = 0;
    cannon->wasShot = SDL_FALSE;
    cannon->shooting = SDL_FALSE;
}

void Cannon_Update()
{
}

void Cannon_Render()
{
}
