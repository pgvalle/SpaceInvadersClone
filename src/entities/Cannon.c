#include "../App.h"
#include <stdio.h>

#define GET_APP_AND_CANNON \
    struct App* app = GetAppInstance();\
    struct Cannon* cannon = &app->game.cannon;

void Cannon_Initialize()
{
    struct Cannon* cannon = &GetAppInstance()->game.cannon;
    cannon->x = CANNON_X_INIT;
    cannon->lives = CANNON_LIVES_TOTAL;
    cannon->shotTimer = 0;
    cannon->deathTimer = 0;
    cannon->deathFrame = 0;
    cannon->deathFrameTimer = 0;
    cannon->wasShot = SDL_FALSE;
    cannon->shooting = SDL_FALSE;
}

void Cannon_Update()
{
    GET_APP_AND_CANNON;

    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RETURN])
        cannon->wasShot = SDL_TRUE;

    if (cannon->wasShot)
    {
        cannon->deathTimer += app->frameTime;
        if (cannon->deathTimer >= CANNON_DEATH_DELTA)
        {
            cannon->wasShot = SDL_FALSE;
            cannon->deathTimer = 0;
        }
        cannon->deathFrameTimer += app->frameTime;
        if (cannon->deathFrameTimer >= CANNON_DEATH_FRAME_DELTA)
        {
            cannon->deathFrame = !cannon->deathFrame;
            cannon->deathFrameTimer = 0;
        }
    }
    else
    {
        if (keys[SDL_SCANCODE_LEFT])
            cannon->x -= 1;
        if (keys[SDL_SCANCODE_RIGHT])
            cannon->x += 1;
    }
}

void Cannon_Render()
{
    GET_APP_AND_CANNON;

    if (!cannon->wasShot)
    {
        SDL_Rect clip = { 0,0, 16,8 };
        SDL_Rect scale = { APP_SCALE(cannon->x),APP_SCALE(CANNON_Y), APP_SCALE(16),APP_SCALE(8) };
        SDL_RenderCopy(app->renderer, app->textures[CANNON_TEX_INDEX], &clip, &scale);
    }
    else if (cannon->deathTimer < CANNON_DEATH_DELTA)
    {
        SDL_Rect clip = {16, 0, 16, 8};
        SDL_Rect scale = { APP_SCALE(cannon->x),APP_SCALE(CANNON_Y), APP_SCALE(16),APP_SCALE(8) };
        if (cannon->deathFrame == 1)
            clip.x = 32;
        SDL_RenderCopy(app->renderer, app->textures[CANNON_TEX_INDEX], &clip, &scale);
        // render death
    }
}
