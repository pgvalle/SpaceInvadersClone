#include "../App.h"
#include <stdio.h>

#define GET_APP_AND_CANNON \
    struct App* app = GetAppInstance();\
    struct Cannon* cannon = &app->game.cannon;

void InitCannon()
{
    struct Cannon* cannon = &GetAppInstance()->game.cannon;
    cannon->x = CANNON_X_INIT;

    cannon->dead = false;
    cannon->deaths = 0;
    cannon->deathTimer = 0;
    cannon->deathFrame = 0;
    cannon->deathFrameTimer = 0;

    cannon->shooting = false;
    cannon->shotTimer = 0;
}

void UpdateCannon()
{
    GET_APP_AND_CANNON;

    cannon->shooting = false; // assume cannon isn't shooting

    if (cannon->dead)
    {
        // update death animation
        cannon->deathFrameTimer += app->frameTime;
        if (cannon->deathFrameTimer >= CANNON_SWAP_DEATH_FRAME_TIMEOUT)
        {
            cannon->deathFrame = !cannon->deathFrame;
            cannon->deathFrameTimer = 0;
        }
        // update death
        cannon->deathTimer += app->frameTime;
        if (cannon->deathTimer >= CANNON_DEATH_TIMEOUT)
        {
            cannon->deaths++;
            // reset death timer stuff entirely
            cannon->deathFrame = 0;
            cannon->deathTimer = 0;
            cannon->deathFrameTimer = 0;
        }
    }
    else // not dead
    {
        // update position with keyboard input
        GET_APP_KEYS;
        if (keys[SDL_SCANCODE_LEFT])
            cannon->x -= 1;
        if (keys[SDL_SCANCODE_RIGHT])
            cannon->x += 1;

        // update shot mechanic
        if (cannon->shotTimer < CANNON_SHOT_COOLDOWN)
            cannon->shotTimer += app->frameTime;
        else if (keys[SDL_SCANCODE_SPACE])
        {
            cannon->shotTimer = 0;
            cannon->shooting = true;
        }
    }
}

void RenderCannon()
{
    GET_APP_AND_CANNON;

    if (!cannon->dead)
    {
        SDL_Rect clip = { 0,0, 16,8 };
        SDL_Rect scale = { APP_SCALE(cannon->x),APP_SCALE(CANNON_Y), APP_SCALE(16),APP_SCALE(8) };
        SDL_RenderCopy(app->renderer, app->textures[TEXINDEX_CANNON], &clip, &scale);
    }
    else if (cannon->deathTimer < CANNON_DEATH_TIMEOUT)
    {
        SDL_Rect clip = { (cannon->deathFrame ? 32 : 16),0, 16,8 };
        SDL_Rect scale = { APP_SCALE(cannon->x),APP_SCALE(CANNON_Y), APP_SCALE(16),APP_SCALE(8) };
        SDL_RenderCopy(app->renderer, app->textures[TEXINDEX_CANNON], &clip, &scale);
    }
}
