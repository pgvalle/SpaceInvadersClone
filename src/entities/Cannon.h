#ifndef CANNON_H_INCLUDED
#define CANNON_H_INCLUDED

#include <stdbool.h>

#define CANNON_X_INIT 26
#define CANNON_Y      220 // won't change

#define CANNON_SHOT_COOLDOWN            1000
#define CANNON_DEATH_TIMEOUT            1000
#define CANNON_SWAP_DEATH_FRAME_TIMEOUT 100

#define CANNON_LIVES_TOTAL 3

struct Cannon
{
    int x; // !!!

    bool dead;  // !!!
    int deaths; // !!!
    int deathTimer;
    int deathFrame;
    int deathFrameTimer;
    
    bool shooting; // !!!
    int shotTimer;
};

void InitCannon();
void UpdateCannon();
void RenderCannon();

#endif // CANNON_H_INCLUDED