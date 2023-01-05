#ifndef RENDER_H
#define RENDER_H

#include <SDL_render.h>

void LoadTextures();
void DestroyTextures();

// x and y are in-game coordinates.
// color is in RGBA format.
void rect_filled_render(int x, int y, int w, int h, Uint32 color);

// x and y are in-game coordinates.
// character set: ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*?
// All other characters are interpreted as spaces.
// So, for example: "i$love$u" gets rendered as "i love u"
void text_white_render(int x, int y, const char* text);
void text_red_render(int x, int y, const char* text);

// NOTE: there isn't CLIP_BUNKER because it's an unique entity.
// Take a look at atlas.png. You won't find a bunker sprite there.
typedef enum clip_t {
    CLIP_TOURIST,
    CLIP_TOURIST_EXPLOSION,

    CLIP_CANNON,
    CLIP_CANNON_EXPLOSION0,
    CLIP_CANNON_EXPLOSION1,

    CLIP_CANNON_SHOT,
    CLIP_CANNON_SHOT_EXPLOSION,

    CLIP_INVADER00,
    CLIP_INVADER01,
    CLIP_INVADER10,
    CLIP_INVADER11,
    CLIP_INVADER20,
    CLIP_INVADER21,
    CLIP_INVADER_EXPLOSION,

    CLIP_COUNT
} clip_t;

// x and y are in-game coordinates.
void clip_render(int x, int y, clip_t clip);


#endif // RENDER_H
