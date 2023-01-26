#include "tourist.h"
#include "game/globals.h"
#include "game/internal.h"
#include "core.h"
#include "utils/random.h"
#include <math.h>

struct tourist_t tourist;

static inline
uint32_t get_spawn_timeout_ms()
{
    return 17 * 60 * random_get_range(
        TOURIST_SPAWN_TIME_MIN_SEC,
        TOURIST_SPAWN_TIME_MAX_SEC
    );
}


void tourist_init()
{
    tourist.state = TOURIST_UNAVAILABLE;

    tourist.x = -1000.f;
    tourist.spawnTimer = (t1mer_t){
        .has_timed_out = false,
        .time = 0,
        .timeout = get_spawn_timeout_ms()
    };
}

void tourist_update()
{
    switch (tourist.state)
    {
    case TOURIST_AVAILABLE_LEFT:
    case TOURIST_AVAILABLE_RIGHT:
        tourist.x += tourist.state == TOURIST_AVAILABLE_LEFT ?
            TOURIST_VEL : -TOURIST_VEL;
        if (tourist.x <= 8.f || tourist.x >= WORLD_WIDTH - 32.f)
        {
            tourist.state = TOURIST_UNAVAILABLE;
            tourist.spawnTimer = (t1mer_t){
                .has_timed_out = false,
                .time = 0,
                .timeout = get_spawn_timeout_ms()
            };
        }
        break;
    case TOURIST_UNAVAILABLE:
        timer_update(&tourist.spawnTimer);
        if (tourist.spawnTimer.has_timed_out) // spawn
        {
            tourist.state = random_get_range(0, 1) ?
                TOURIST_AVAILABLE_LEFT : TOURIST_AVAILABLE_RIGHT;
            tourist.x = tourist.state == TOURIST_AVAILABLE_LEFT ?
                8.f : (WORLD_WIDTH - 32.f);
        }
        break;
    }
}

void tourist_render()
{
    if (tourist.state == TOURIST_UNAVAILABLE)
        return;
    
    static const SDL_Rect clip = {  0,  0, 24,  8 };
    const SDL_Rect scale = {
        APP_SCALE * roundf(tourist.x),
        APP_SCALE * TOURIST_Y,
        APP_SCALE * clip.w,
        APP_SCALE * clip.h
    };

    SDL_RenderCopy(
        app.renderer,
        atlas,
        &clip,
        &scale
    );
}
