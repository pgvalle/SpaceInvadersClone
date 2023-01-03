#include "Internal.h"

struct Bunker bunkers[4];

void InitBunker(int k)
{
    // top
    for (int i = 0; i < 4; i++)
    {
        for (int j = 4 - i; j < 18 + i; j++)
        {
            bunkers[k].pieces[22 * i + j].x = BUNKER_FIRST_X + 46 * k + j;
            bunkers[k].pieces[22 * i + j].y = BUNKERS_Y + i;
        }
    }

    // middle
    for (int i = 4; i < 12; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            bunkers[k].pieces[22 * i + j].x = BUNKER_FIRST_X + 46 * k + j;
            bunkers[k].pieces[22 * i + j].y = BUNKERS_Y + i;
        }
    }

    for (int j = 0; j < 8; j++)
    {
        bunkers[k].pieces[22 * 12 + j].x = BUNKER_FIRST_X + 46 * k + j;
        bunkers[k].pieces[22 * 12 + j].y = BUNKERS_Y + 12;

        bunkers[k].pieces[22 * 12 + j + 14].x = BUNKER_FIRST_X + 46 * k + j + 14;
        bunkers[k].pieces[22 * 12 + j + 14].y = BUNKERS_Y + 12;
    }

    for (int j = 0; j < 7; j++)
    {
        bunkers[k].pieces[22 * 13 + j].x = BUNKER_FIRST_X + 46 * k + j;
        bunkers[k].pieces[22 * 13 + j].y = BUNKERS_Y + 13;

        bunkers[k].pieces[22 * 13 + j + 15].x = BUNKER_FIRST_X + 46 * k + j + 15;
        bunkers[k].pieces[22 * 13 + j + 15].y = BUNKERS_Y + 13;
    }

    for (int i = 14; i < 16; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            bunkers[k].pieces[22 * i + j].x = BUNKER_FIRST_X + 46 * k + j;
            bunkers[k].pieces[22 * i + j].y = BUNKERS_Y + i;

            bunkers[k].pieces[22 * i + j + 16].x = BUNKER_FIRST_X + 46 * k + j + 16;
            bunkers[k].pieces[22 * i + j + 16].y = BUNKERS_Y + i;
        }
    }
}

void InitBunkers()
{
    // upper part
    for (int k = 0; k < 4; k++)
        InitBunker(k);
}

void RenderBunkers()
{
    SDL_SetRenderDrawColor(app.renderer, 32, 255, 32, 255);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 352; j++)
        {
            const SDL_Rect pieceRect = {
                bunkers[i].pieces[j].x * app.options.scale,
                bunkers[i].pieces[j].y * app.options.scale,
                app.options.scale,
                app.options.scale
            };

            SDL_RenderFillRect(app.renderer, &pieceRect);
        }
    }
}