#include "../App.h"

#define GET_APP_AND_TOURIST \
    struct App* app = GetAppInstance();\
    struct Tourist* tourist = &app->game.tourist;

void UpdateTourist()
{
    GET_APP_AND_TOURIST;

    if (!tourist->spawned)
    {
        // update spawn timer
        tourist->spawnTimer += app->frameTime;
        if (tourist->spawnTimer >= tourist->spawnTimeout)
            // Done waiting. Spawn!
        {
            // set spawning location settings
            tourist->spawnedRight = rand() % 2 ? true : false;
            // FIXME: use more precise clipping values
            if (tourist->spawnedRight)
                tourist->x = 0;
            else
                tourist->x = APP_VSCREEN_WIDTH - 10;

            tourist->spawned = true; // now it has spawned
            tourist->dead = false; // can't spawn dead. DUH          
            // reset spawn timer with random timeout
            tourist->spawnTimer = 0;
            tourist->spawnTimeout = (rand() % 30000) + 30000;
        }
    }
    else if (tourist->dead) // spawned and dead
    {
        // update death timer
        tourist->deathTimer += app->frameTime;
        if (tourist->deathTimer >= TOURIST_DEATH_TIMEOUT)
            // death timeout done. Execute spawning logic
        {
            tourist->deathTimer = 0;
            tourist->spawned = false;
        }
    }
    else // spawned and not dead
    {
        // Update position
        int moveOffset = tourist->spawnedRight ? 1 : -1;
        tourist->x += moveOffset;
        // FIXME: use more precise clipping values 2
        if (tourist->x < 0 || tourist->x >= APP_VSCREEN_WIDTH - 10)
            // out of bounds. Excecute spawning logic
            tourist->spawned = false;
    }
}

void RenderTourist()
{
    GET_APP_AND_TOURIST;

    // if not spawned don't render anything
    if (!tourist->spawned)
        return;

    if (!tourist->dead)
    {
        SDL_Rect clip = { 0, 0, 24, 8 };
        SDL_Rect scale = { APP_SCALE(tourist->x),APP_SCALE(TOURIST_Y), APP_SCALE(24),APP_SCALE(8) };
        SDL_RenderCopy(app->renderer, app->textures[TEXINDEX_TOURIST], &clip, &scale);
    }
    else if (tourist->deathTimer < TOURIST_DEATH_TIMEOUT)
    {
        SDL_Rect clip = { 24, 0, 24, 8 };
        SDL_Rect scale = { APP_SCALE(tourist->x),APP_SCALE(TOURIST_Y), APP_SCALE(24),APP_SCALE(8) };
        SDL_RenderCopy(app->renderer, app->textures[TEXINDEX_TOURIST], &clip, &scale);
    }
}