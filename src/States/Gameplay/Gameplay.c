#include "Internal.h"
#include "../StatesInternal.h"

bool gamePaused;
bool startAnimation;
int startAnimationSteps;

void InitGlobals()
{
    gamePaused = false;
    startAnimation = true;
    startAnimationSteps = 0;
}

void UpdateGlobals()
{
    if (app.event.type == SDL_KEYDOWN && !app.event.key.repeat)
    {
        if (gamePaused && app.event.key.keysym.sym == SDLK_q)
            PopState();

        if (app.event.key.keysym.sym == SDLK_ESCAPE)
            gamePaused = !gamePaused;
    }

    if (!startAnimation || gamePaused)
        return;

    // Only executes if the game isn't paused and start animation is playing

    // "undeading" invaders starting from bottom-left
    const int i = 4 - startAnimationSteps / 11;
    const int j = startAnimationSteps % 11;
    horde.invaders[11 * i + j].dead = false;

    // updating animation status
    if (++startAnimationSteps == HORDE_INVADER_COUNT)
        startAnimation = false;
}

void InitGameplayState()
{
    SDL_Log("Inside gameplay state...");

    InitGlobals();
    InitHorde();
    InitTourist();
    InitCannon();
    InitBunkers();
}

void DestroyGameplayState()
{

}

void UpdateGameplayState()
{
    UpdateGlobals();

    // Don't update if starting animation is being played or the game is paused
    if (gamePaused || startAnimation)
        return;

    UpdateShots();
    ProcessHordeCollisions();
    ProcessTouristCollision();
    ProcessBunkersCollisions();
    UpdateExplosions();
    UpdateCannon();
    MoveHorde();
    UpdateTourist();
}

void RenderGameplayState()
{
    RenderShots();
    RenderHorde();
    RenderTourist();
    RenderCannon();
    RenderExplosions();
    RenderBunkers();

    RenderWhiteText(8, 168 + HORDE_Y_INIT, "3");
}