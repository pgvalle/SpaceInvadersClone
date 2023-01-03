#include "Animation.h"
#include "Renderer.h"
#include "stb_ds.h"
#include <stdarg.h>

void InitAnimation(Animation* anim, int size, ...)
{
    anim->frames = NULL;
    anim->size = size;
    anim->current = 0;

    va_list args;
    va_start(args, size);
    for (int i = 0; i < size; i++)
        arrput(anim->frames, va_arg(args, AnimationFrame));
    va_end(args);
}

void FreeAnimation(Animation* anim)
{
    arrfree(anim->frames);
    anim->size = 0;
    anim->current = 0;
}

void UpdateAnimation(Animation* anim)
{
    const int current = anim->current;

    UpdateTimer(&anim->frames[current].timer);
    if (anim->frames[current].timer.reachedTimeout)
    {
        // only update frame pointer if it is valid
        if (current + 1 <= anim->size)
            anim->current++;
    }
}
