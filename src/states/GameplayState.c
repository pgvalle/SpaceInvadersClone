#include "StatesInternal.h"
#include "../Application.h"
#include "../entities/Entities.h"

#define START_ANIMATION_PACE 35

struct Horde horde;
struct Cannon cannon;

bool startAnimation;
int startAnimationI;
int startAnimationJ;
struct Timer startAnimationTimer;

void InitGameplayState()
{
    InitHorde(&horde);
    InitCannon(&cannon);

    startAnimation = true;
    startAnimationI = 4;
    startAnimationJ = 0;
    startAnimationTimer = (struct Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = START_ANIMATION_PACE
    };
}

void DestroyGameplayState()
{
    
}

void UpdateGameplayState()
{
    if (startAnimation)
    {
        // update start animation timer
        UpdateTimer(&startAnimationTimer);
        if (startAnimationTimer.reachedTimeout)
        {
            int k = startAnimationI * 11 + startAnimationJ;
            startAnimationJ++;
            if (startAnimationJ == 11)
            {
                startAnimationJ = 0;
                startAnimationI--;
            }
            horde.invaders[k].dead = false;
            horde.invaders[k].deathTimer.reachedTimeout = false;

            if (--horde.deadCount == 0)
                startAnimation = false;
        }
    }
    else
    {
        UpdateHorde(&horde);
        UpdateCannon(&cannon);
    }
}

void RenderGameplayState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    RenderHorde(&horde);
    RenderCannon(&cannon);

    SDL_RenderPresent(app.renderer);
}