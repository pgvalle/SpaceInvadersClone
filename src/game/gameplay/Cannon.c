#include "Internal.h"

struct Cannon cannon;

void InitCannon()
{
    cannon.x = CANNON_X_INIT;

    cannon.dead = false;
    cannon.deaths = 0;

    cannon.shotTimer = (t1mer_t){
        .has_timed_out = false,
        .time = 0,
        .timeout = CANNON_SHOT_COOLDOWN
    };
}

void UpdateCannon()
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT] && cannon.x > CANNON_X_INIT)
        cannon.x -= 1;
    if (keys[SDL_SCANCODE_RIGHT] && cannon.x < WORLD_WIDTH - 31)
        cannon.x += 1;

    if (!cannon.shotTimer.has_timed_out)
        timer_update(&cannon.shotTimer);
    if (cannon.shotTimer.has_timed_out && keys[SDL_SCANCODE_SPACE])
    {
        cannon.shotTimer.has_timed_out = false;
        // spawn shot
        struct CannonShot shot = {
            .x = cannon.x + 8,
            .y = CANNON_Y
        };
        arrput(cannonShots, shot);
    }
}

void RenderCannon()
{
    if (cannon.dead)
        return;

    static const SDL_Rect clip = {  0,  8, 16,  8 };
    clip_render(
        &clip,
        ATLAS_INDEX,
        WORLD_WIDTH,
        WORLD_HEIGHT,
        cannon.x,
        CANNON_Y
    );
}