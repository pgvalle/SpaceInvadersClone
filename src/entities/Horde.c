#include "../App.h"

void Horde_Initialize()
{
    struct Horde* horde = &App_Get()->game.horde;
    
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

void Horde_Update()
{
    struct App* app = App_Get();
    struct Horde* horde = &app->game.horde;

    // update move timer
    horde->moveTimer += app->frameTime;
    // cant move yet
    if (horde->locked || horde->moveTimer < horde->moveTimeout)
        return;
    horde->moveTimer = 0;

    if (horde->moveCount == 16)
    {
        for (int i = 0; i < HORDE_SIZE; i++)
        {
            if (horde->array[i].alive)
                horde->array[i].y += 8;
        }

        horde->moveCount = 0;
        horde->goingRight = !horde->goingRight;
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
}

void Horde_Render()
{
    struct App* app = App_Get();
    struct Horde* horde = &app->game.horde;

    SDL_SetRenderDrawColor(app->renderer, 255, 255,255, 255);
    for (int i = 0; i < HORDE_SIZE; i++)
    {
        SDL_Rect rect = { APP_SCALE(horde->array[i].x),APP_SCALE(horde->array[i].y),
            APP_SCALE(8),APP_SCALE(8) };
        SDL_RenderFillRect(app->renderer, &rect);
    }

    SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
}