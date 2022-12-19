#ifndef CANNON_H_INCLUDED
#define CANNON_H_INCLUDED

#include <SDL.h>

#define CANNON_X_INIT 0
#define CANNON_Y      0 // won't change

#define CANNON_MOVEMENT_PACE 1

#define CANNON_SHOT_COOLDOWN 1000
#define CANNON_MOVE_DELTA    16

#define CANNON_LIVES_TOTAL 3

#define CANNON_TEX_INDEX 0

struct Cannon
{
    int x;
    int lives;
    int moveTimer;
    int deathTimer;
    int shotTimer;
    SDL_bool wasShot;
    SDL_bool shooting;
};

void Cannon_Initialize();
void Cannon_Update();
void Cannon_Render();

#endif // CANNON_H_INCLUDED