#include "Entities.h"
#include "../Application.h"
#include <SDL.h>

void GetEntityRect(enum EntityTex tex, SDL_Rect* out)
{
    static const SDL_Rect rectMap[ENTITYTEX_COUNT] =
    {
        {  0,  0, 24,  8 }, // tourist
        { 24,  0, 24,  8 }, // tourist dead

        {  0,  8, 16,  8 }, // cannon
        { 16,  8, 16,  8 }, // cannon dead, frame 0
        { 32,  8, 16,  8 }, // cannon dead, frame 1

        {  0, 16, 12,  8 }, // invader 0, frame 0
        { 12, 16, 12,  8 }, // invader 0, frame 1
        {  0, 24, 12,  8 }, // invader 1, frame 0
        { 12, 24, 12,  8 }, // invader 1, frame 1
        {  0, 32, 12,  8 }, // invader 2, frame 0
        { 12, 32, 12,  8 }, // invader 2, frame 1
        {  0, 40, 13,  8 }, // invader dead
    };

    *out = rectMap[tex];
}

void RenderEntity(int x, int y, enum EntityTex tex)
{
    // get the clip rectangle
    SDL_Rect clip;
    GetEntityRect(tex, &clip);

    // calculate scale rectangle according to app scaling factor
    const int scaleFactor = app.options.scale;
    const SDL_Rect scale = {
        scaleFactor * x,
        scaleFactor * y,
        scaleFactor * clip.w,
        scaleFactor * clip.h
    };

    SDL_RenderCopy(app.renderer, app.entitiesTex, &clip, &scale);
}