#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <stdint.h>

// NOTE: there isn't ENTITYTEX_BUNKER because it's an unique entity.
// Take a look at atlas.png and you won't find a bunker image there.
enum EntityTex
{
    ENTITYTEX_TOURIST,
    ENTITYTEX_TOURIST_DEAD,

    ENTITYTEX_CANNON,
    ENTITYTEX_CANNON_DEAD0,
    ENTITYTEX_CANNON_DEAD1,

    ENTITYTEX_INVADER00,
    ENTITYTEX_INVADER01,
    ENTITYTEX_INVADER10,
    ENTITYTEX_INVADER11,
    ENTITYTEX_INVADER20,
    ENTITYTEX_INVADER21,

    ENTITYTEX_INVADER_DEAD,

    ENTITYTEX_COUNT
};

// x and y are in-game coordinates.
void RenderEntityTex(int x, int y, enum EntityTex tex);

// x and y are in-game coordinates
// character set: ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*?
// All other characters are interpreted as spaces.
// So, for example: "i$love$u" -> "i love u"
void RenderText(int x, int y, const char* text, bool red);
// x and y are in-game coordinates.
void RenderUnsignedInt(int x, int y, uint64_t value, bool red);


#endif // RENDERER_H
