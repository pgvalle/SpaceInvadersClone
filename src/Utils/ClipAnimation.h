#ifndef CLIP_ANIMATION_H
#define CLIP_ANIMATION_H

#include "Renderer.h"
#include "Timer.h"

struct ClipAnimationFrame
{
    AtlasClip clip;
    Timer timer;
};
typedef struct ClipAnimationFrame ClipAnimationFrame;

struct ClipAnimation
{
    ClipAnimationFrame* frames;
    int size;
    int current;
};
typedef struct ClipAnimation ClipAnimation;

// Using stdarg here. Really cool syntax sugar to create animations
void InitClipAnimation(ClipAnimation* anim, int size, ...);
// Frees anim->frames and reset other fields
void FreeClipAnimation(ClipAnimation* anim);

static inline void ResetClipAnimation(ClipAnimation* anim)
{
    anim->current = 0;
}

static inline bool HasClipAnimationFinished(const ClipAnimation* anim)
{
    return anim->size == anim->current;
}

void UpdateClipAnimation(ClipAnimation* anim);
static inline void RenderClipAnimation(int x, int y, const ClipAnimation* anim)
{
    RenderAtlasClip(x, y, anim->frames[anim->current].clip);
}


#endif // CLIP_ANIMATION_H
