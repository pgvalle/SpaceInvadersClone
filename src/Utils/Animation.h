#ifndef ANIMATION_H
#define ANIMATION_H

#include "Render.h"
#include "Timer.h"

struct AnimationFrame
{
    Clip clip;
    Timer timer;
};
typedef struct AnimationFrame AnimationFrame;

struct Animation
{
    AnimationFrame* frames;
    int size;
    int current;
};
typedef struct Animation Animation;

// Using stdarg here. Really cool syntax sugar to create animations
void InitAnimation(Animation* anim, int size, ...);
// Frees anim->frames and reset other fields
void FreeAnimation(Animation* anim);

static inline void ResetAnimation(Animation* anim)
{
    anim->current = 0;
}

static inline bool HasAnimationFinished(const Animation* anim)
{
    return anim->size == anim->current;
}

void UpdateAnimation(Animation* anim);

static inline void RenderAnimation(int x, int y, const Animation* anim)
{
    RenderClip(x, y, anim->frames[anim->current].clip);
}


#endif // ANIMATION_H
