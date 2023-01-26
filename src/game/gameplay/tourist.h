#ifndef TOURIST_H
#define TOURIST_H

#include "utils/timer.h"

#define TOURIST_Y 48

#define TOURIST_VEL 0.65f

#define TOURIST_SPAWN_TIME_MIN_SEC 20
#define TOURIST_SPAWN_TIME_MAX_SEC 30

extern struct tourist_t
{
    enum {
        TOURIST_UNAVAILABLE,
        TOURIST_AVAILABLE_LEFT,
        TOURIST_AVAILABLE_RIGHT
    } state;

    float x;
    t1mer_t spawnTimer;
} tourist;

void tourist_init();
void tourist_update();
void tourist_render();


#endif // TOURIST_H
