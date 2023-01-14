#include "tourist.h"
#include "game/constants.h"
#include "core/app.h"
#include "utils/render.h"
#include <SDL_rect.h>

struct tourist_t tourist;


void tourist_init()
{
    tourist.state = TOURIST_UNAVAILABLE;

    tourist.x = -1000.f;
    tourist.spawnTimer = (t1mer_t){
        .has_timed_out = false,
        .time = 0,
        .timeout = 17 * 60 * random_get_uint32_range(
            &app.random,
            TOURIST_SPAWN_TIME_MIN,
            TOURIST_SPAWN_TIME_MAX
        )
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
            const uint32_t seconds = random_get_uint32_range(
                    &app.random,
                    TOURIST_SPAWN_TIME_MIN,
                    TOURIST_SPAWN_TIME_MAX
                );
                SDL_LogInfo(0, "%u\n", seconds);
            tourist.state = TOURIST_UNAVAILABLE;
            tourist.spawnTimer = (t1mer_t){
                .has_timed_out = false,
                .time = 0,
                .timeout = 17 * 60 * seconds
            };
        }
        break;
    case TOURIST_UNAVAILABLE:
        timer_update(&tourist.spawnTimer);
        if (tourist.spawnTimer.has_timed_out) // spawn
        {
            tourist.state = rand() % 2 ?
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
    clip_render(
        &clip, ATLAS_INDEX, WORLD_WIDTH, WORLD_HEIGHT, tourist.x, TOURIST_Y
    );
}
