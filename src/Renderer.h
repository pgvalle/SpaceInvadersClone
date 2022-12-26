#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>

// NOTE: there isn't ENTITYTEX_BUNKER because it's an unique entity.
// Take a look at atlas.png and you won't find a bunker texture there.
enum EntityTexEnum
{
    ENTITYTEX_TOURIST,
    ENTITYTEX_TOURIST_DEAD,

    ENTITYTEX_CANNON,
    ENTITYTEX_DEAD0,
    ENTITYTEX_DEAD1,

    ENTITYTEX_INVADER00,
    ENTITYTEX_INVADER01,
    ENTITYTEX_INVADER10,
    ENTITYTEX_INVADER11,
    ENTITYTEX_INVADER20,
    ENTITYTEX_INVADER21,

    ENTITYTEX_INVADER_DEAD,

    ENTITYTEX_COUNT
};

// x and y are in game coordinates. Not screen coordinates.
void RenderEntity(int x, int y, enum EntityTexEnum tex);

// x and y are in game coordinates. Not screen coordinates.
// Non-alphanumeric characters are rendered as whitespaces.
void RenderText(int x, int y, const char* text, bool red);
// x and y are in game coordinates. Not screen coordinates.
void RenderInt(int x, int y, int value, bool red);


#endif // RENDERER_H
