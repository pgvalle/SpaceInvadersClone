#ifndef TOURIST_H_INCLUDED
#define TOURIST_H_INCLUDED

#include <stdbool.h>

#define TOURIST_Y 10

#define TOURIST_DEATH_TIMEOUT 1000

struct Tourist
{
    int x;

    bool dead; // updated from interaction with Shot entities
    int deathTimer;

    bool spawnedRight;
    bool spawned;
    int spawnTimer;
    int spawnTimeout;

    int scoreValue;
};

void UpdateTourist();
void RenderTourist();

#endif // TOURIST_H_INCLUDED