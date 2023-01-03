#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>

// NOTE: there isn't ATLASCLIP_BUNKER because it's an unique entity.
// Take a look at atlas.png and you won't find a bunker image there.
enum AtlasClip
{
    ATLASCLIP_TOURIST,
    ATLASCLIP_TOURIST_EXPLOSION,

    ATLASCLIP_CANNON,
    ATLASCLIP_CANNON_EXPLOSION0,
    ATLASCLIP_CANNON_EXPLOSION1,

    ATLASCLIP_CANNON_SHOT,
    ATLASCLIP_CANNON_SHOT_EXPLOSION,

    ATLASCLIP_INVADER00,
    ATLASCLIP_INVADER01,
    ATLASCLIP_INVADER10,
    ATLASCLIP_INVADER11,
    ATLASCLIP_INVADER20,
    ATLASCLIP_INVADER21,
    ATLASCLIP_INVADER_EXPLOSION,

    ATLASCLIP_COUNT
};
typedef enum AtlasClip AtlasClip;

// x and y are in-game coordinates.
void RenderAtlasClip(int x, int y, AtlasClip clip);

// x and y are in-game coordinates.
// character set: ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*?
// All other characters are interpreted as spaces.
// So, for example: "i$love$u" -> "i love u"
void RenderText(int x, int y, const char* text, bool red);


#endif // RENDERER_H
