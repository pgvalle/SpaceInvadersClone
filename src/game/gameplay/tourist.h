#ifndef TOURIST_H
#define TOURIST_H

#include "utils/timer.h"

#define TOURIST_Y 48

#define TOURIST_VEL 0.65f

#define TOURIST_SPAWN_TIME_MIN 20
#define TOURIST_SPAWN_TIME_MAX 30

enum tourist_state_t
{
    TOURIST_UNAVAILABLE,
    TOURIST_AVAILABLE_LEFT,
    TOURIST_AVAILABLE_RIGHT,
};

extern struct tourist_t
{
    enum tourist_state_t state;

    float x;
    t1mer_t spawnTimer;
} tourist;

void tourist_init();
void tourist_update();
void tourist_render();


#endif // TOURIST_H
