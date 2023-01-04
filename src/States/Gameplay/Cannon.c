#include "Internal.h"

struct Cannon cannon;

void InitCannon()
{
    cannon.x = CANNON_X_INIT;

    cannon.dead = false;
    cannon.deaths = 0;

    cannon.shotTimer = (Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = CANNON_SHOT_COOLDOWN
    };
}

void UpdateCannon()
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_LEFT] && cannon.x > CANNON_X_INIT)
        cannon.x -= 1;
    if (keys[SDL_SCANCODE_RIGHT] && cannon.x < APP_VSCREEN_WIDTH - 31)
        cannon.x += 1;

    if (!cannon.shotTimer.reachedTimeout)
        UpdateTimer(&cannon.shotTimer);
    if (cannon.shotTimer.reachedTimeout && keys[SDL_SCANCODE_SPACE])
    {
        cannon.shotTimer.reachedTimeout = false;
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
    if (!cannon.dead)
        RenderClip(cannon.x, CANNON_Y, CLIP_CANNON);
}