#include "StatesInternal.h"
#include "../Application.h"
#include "../entities/Entities.h"

#define START_ANIMATION_PACE 50

struct Horde horde;
struct Cannon cannon;

bool startAnimation;
int startAnimationI;
int startAnimationJ;
int startAnimationTimer;

void InitGameplayState()
{
    InitHorde(&horde);
    InitCannon(&cannon);

    startAnimation = true;
    startAnimationI = 4;
    startAnimationJ = 0;
    startAnimationTimer = 0;
}

void DestroyGameplayState()
{
    
}

void UpdateGameplayState()
{
    if (startAnimation)
    {
        startAnimationTimer += app.frameTime;
        if (startAnimationTimer >= START_ANIMATION_PACE)
        {
            int k = startAnimationI * 11 + startAnimationJ;
            startAnimationJ++;
            if (startAnimationJ == 11)
            {
                startAnimationJ = 0;
                startAnimationI--;
            }
            horde.invaders[k].dead = false;
            horde.invaders[k].deathTimer = 0;
            startAnimationTimer = 0;
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