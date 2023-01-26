#include "animation.h"
#include "stb_ds.h"
#include "core/app.h"
#include <stdarg.h>

static inline
animation_frame_t* animation_get_current_frame(const animation_t* anim)
{
    return &anim->frames[anim->current];
}

void animation_init(animation_t* anim, SDL_Texture* texture, uint8_t size, ...)
{
    anim->texture = texture;
    anim->frames = NULL;
    anim->current = 0;

    va_list frame_list;
    va_start(frame_list, size);
    for (int i = 0; i < size; i++)
        arrput(anim->frames, va_arg(frame_list, animation_frame_t));
    va_end(frame_list);
}

void animation_free(animation_t* anim)
{
    anim->texture = NULL;
    arrfree(anim->frames);
    anim->current = 0;
}

bool animation_is_over(const animation_t* anim)
{
    return anim->current == arrlen(anim->frames);
}

void animation_update(animation_t* anim)
{
    timer_update(&anim->frames[anim->current].timer);
    if (anim->frames[anim->current].timer.has_timed_out)
    {
        // only update frame pointer if it is valid
        if (anim->current + 1 < arrlen(anim->frames))
            anim->current++;
    }
}

void animation_render(const animation_t* anim, int x, int y)
{
    const SDL_Rect clip = anim->frames[anim->current].clip;
    const SDL_Rect scale = {
        APP_SCALE * x,
        APP_SCALE * y,
        APP_SCALE * clip.w,
        APP_SCALE * clip.h
    };

    SDL_RenderCopy(app.renderer, anim->texture, &clip, &scale);
}
