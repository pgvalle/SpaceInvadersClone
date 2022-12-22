#ifndef TOURIST_H_INCLUDED
#define TOURIST_H_INCLUDED

#include <stdbool.h>

#define TOURIST_Y 10

#define TOURIST_DEATH_TIMEOUT 1000

#define TOURIST_MOVEANIM_PACE 25

struct Tourist
{
    int x; // !!!

    bool dead; // !!!
    int deathTimer;

    bool spawned; // !!!
    bool spawnedRight;
    int spawnTimer;
    int spawnTimeout;

    int moveAnimationTimer;

    int scoreValue; // !!!
};

void UpdateTourist();
void RenderTourist();

#endif // TOURIST_H_INCLUDED