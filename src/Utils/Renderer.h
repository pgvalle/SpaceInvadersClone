#ifndef RENDERER_H
#define RENDERER_H

// NOTE: there isn't CLIP_BUNKER because it's an unique entity.
// Take a look at atlas.png. You won't find a bunker sprite there.
enum Clip
{
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
};
typedef enum Clip Clip;

// x and y are in-game coordinates.
void RenderClip(int x, int y, Clip clip);

// x and y are in-game coordinates.
// character set: ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*?
// All other characters are interpreted as spaces.
// So, for example: "i$love$u" gets rendered as "i love u"
void RenderWhiteText(int x, int y, const char* text),
     RenderRedText  (int x, int y, const char* text);


#endif // RENDERER_H
