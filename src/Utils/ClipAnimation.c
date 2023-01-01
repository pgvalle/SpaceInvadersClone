#include "ClipAnimation.h"
#include "Renderer.h"
#include "stb_ds.h"
#include <stdarg.h>

void InitClipAnimation(struct ClipAnimation* anim, int frameCount, ...)
{
    anim->frames = NULL;
    anim->frameCount = frameCount;
    anim->currentFrame = 0;

    va_list args;
    va_start(args, frameCount);
    for (int i = 0; i < frameCount; i++)
        arrput(anim->frames, va_arg(args, struct ClipAnimationFrame));
    va_end(args);
}

void UpdateClipAnimation(struct ClipAnimation* anim)
{
    const int current = anim->currentFrame;

    UpdateTimer(&anim->frames[current].timer);
    if (anim->frames[current].timer.reachedTimeout)
    {
        // only update frame pointer if it is valid
        if (current + 1 <= anim->frameCount)
            anim->currentFrame++;
    }
}

void RenderClipAnimation(int x, int y, const struct ClipAnimation* anim)
{
    RenderAtlasClip(x, y, anim->frames[anim->currentFrame].clip);
}
