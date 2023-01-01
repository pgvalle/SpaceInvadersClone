#ifndef CLIP_ANIMATION_H
#define CLIP_ANIMATION_H

#include "Timer.h"

struct ClipAnimationFrame
{
    enum AtlasClip clip;
    struct Timer timer;
};

struct ClipAnimation
{
    struct ClipAnimationFrame* frames;
    int frameCount;
    int currentFrame;
};

void InitClipAnimation(struct ClipAnimation* anim, int frameCount, ...);

inline void ResetClipAnimation(struct ClipAnimation* anim)
{
    anim->currentFrame = 0;
}

inline bool HasClipAnimationFinished(const struct ClipAnimation* anim)
{
    return anim->frameCount == anim->currentFrame;
}

void UpdateClipAnimation(struct ClipAnimation* anim);

void RenderClipAnimation(int x, int y, const struct ClipAnimation* anim);


#endif // CLIP_ANIMATION_H
