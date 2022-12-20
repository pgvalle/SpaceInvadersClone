#include "../App.h"
#include <stdlib.h>

#define GET_APP_AND_HORDE \
struct App* app = App_Get();\
struct Horde* horde = &app->game.horde;

void Horde_Initialize()
{
    GET_APP_AND_HORDE;
    
    for (int i = 0; i < 11; i++)
    {
        // first row
        horde->array[i].type = INVADER_0;
        horde->array[i].x = HORDE_X_INIT + 16 * i;
        horde->array[i].y = HORDE_Y_INIT;
        // second
        horde->array[11 + i].type = INVADER_1;
        horde->array[11 + i].x = HORDE_X_INIT + 16 * i;
        horde->array[11 + i].y = HORDE_Y_INIT + 16;
        // third
        horde->array[22 + i].type = INVADER_1;
        horde->array[22 + i].x = HORDE_X_INIT + 16 * i;
        horde->array[22 + i].y = HORDE_Y_INIT + 32;
        // fourth
        horde->array[33 + i].type = INVADER_2;
        horde->array[33 + i].x = HORDE_X_INIT + 16 * i;
        horde->array[33 + i].y = HORDE_Y_INIT + 48;
        // fifth
        horde->array[44 + i].type = INVADER_2;
        horde->array[44 + i].x = HORDE_X_INIT + 16 * i;
        horde->array[44 + i].y = HORDE_Y_INIT + 64;
    }

    // other invader fields
    for (int i = 0; i < HORDE_SIZE; i++)
    {
        horde->array[i].alive = SDL_TRUE;
        horde->array[i].hasMoved = SDL_FALSE;
        horde->array[i].deathTimer = 0;
    }

    horde->locked = SDL_FALSE;
    horde->goingRight = SDL_TRUE;
    horde->moveCount = 8;
    horde->moveTimer = 0;
    horde->moveTimeout = HORDE_MOVE_DELTA_INIT;
}

void Horde_UpdateDeadInvaders()
{
    GET_APP_AND_HORDE;

    // assume horde has dead invaders with death animation unfinished
    horde->locked = SDL_TRUE;

    int deadCount = 0;
    int deadAndFinishedCount = 0;
    for (int i = 0; i < HORDE_SIZE; i++)
    {
        // don't care
        if (horde->array[i].alive)
            continue;
        
        deadCount++; // +1 dead
        // update death timer
        if (horde->array[i].deathTimer < INVADER_DEATH_DELTA)
            horde->array[i].deathTimer += app->frameTime;
        else // didn't update, then its over for this specific invader
            deadAndFinishedCount++;
    }

    // if this happens, it means all dead invaders dont matter anymore
    if (deadAndFinishedCount == deadCount)
        horde->locked = SDL_FALSE;
}

void Horde_Update()
{
    GET_APP_AND_HORDE;

    if (app->hadEvent && app->event.type == SDL_KEYDOWN &&
        app->event.key.keysym.sym == SDLK_SPACE)
    {
        int i = rand() % HORDE_SIZE;
        horde->array[i].alive = SDL_FALSE;
    }

    Horde_UpdateDeadInvaders();

    // update move timer
    horde->moveTimer += app->frameTime;
    // cant move yet
    if (horde->locked || horde->moveTimer < horde->moveTimeout)
        return;
    horde->moveTimer = 0;

    // should move down
    if (horde->moveCount == 16)
    {
        for (int i = 0; i < HORDE_SIZE; i++)
        {
            if (horde->array[i].alive)
                horde->array[i].y += 8;
        }

        horde->moveCount = 0; // reset move counter
        horde->goingRight = !horde->goingRight; // change horizontal movement
    }
    else
    {
        int offset = horde->goingRight ? 2 : -2;
        for (int i = 0; i < HORDE_SIZE; i++)
        {
            if (horde->array[i].alive)
                horde->array[i].x += offset;
        }

        horde->moveCount++;
    }

    // change animation frame for all invaders in horde
    horde->moveClip = !horde->moveClip;
}

void Horde_Render()
{
    GET_APP_AND_HORDE;

    for (int i = 0; i < HORDE_SIZE; i++)
    {
        int x = horde->array[i].x;
        int y = horde->array[i].y;

        if (horde->array[i].alive)
        {
            SDL_Rect clip = { 0,0, 12,8 };
            SDL_Rect scale = { APP_SCALE(x),APP_SCALE(y), APP_SCALE(12),APP_SCALE(8) };
            if (horde->moveClip == 1)
                clip.x = 12;
            if (horde->array[i].type == INVADER_1)
                clip.y = 8;
            else if (horde->array[i].type == INVADER_2)
                clip.y = 16;
            SDL_RenderCopy(app->renderer, app->textures[INVADERS_TEX_INDEX], &clip, &scale);
        }
        else if (horde->array[i].deathTimer < INVADER_DEATH_DELTA)
        {
            SDL_Rect clip = { 0,24, 13,8 };
            SDL_Rect scale = { APP_SCALE(x - 1),APP_SCALE(y), APP_SCALE(13),APP_SCALE(8) };
            SDL_RenderCopy(app->renderer, app->textures[INVADERS_TEX_INDEX], &clip, &scale);
        }
    }
}