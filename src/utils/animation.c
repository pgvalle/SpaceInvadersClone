#include "animation.h"
#include "stb_ds.h"
#include "../core.h"
#include <stdarg.h>

static inline
animation_frame_t* animation_get_current_frame(const animation_t* anim)
{
    return &anim->frames[anim->current];
}

void animation_init(animation_t* anim, uint8_t texture_id, uint8_t size, ...)
{
    anim->texture_id = texture_id;
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
    anim->texture_id = 0;
    arrfree(anim->frames);
    anim->current = 0;
}

bool animation_is_over(const animation_t* anim)
{
    return anim->current == arrlen(anim->frames);
}

void animation_update(animation_t* anim)
{
    timer_update(&animation_get_current_frame(anim)->timer);
    if (animation_get_current_frame(anim)->timer.has_timed_out)
    {
        // only update frame pointer if it is valid
        if (anim->current < arrlen(anim->frames))
            anim->current++;
    }
}

void animation_render(const animation_t* anim, int x, int y)
{
    const int factor = app.fullscreen ? app.fs_scale : app.scale;
    const SDL_Rect clip = animation_get_current_frame(anim)->clip;
    const SDL_Rect scale = {
        factor * x, factor * y, factor * clip.w, factor * clip.h
    };

    SDL_RenderCopy(
        app.renderer,
        asset_man_get(ASSETTYPE_TEXTURE, anim->texture_id),
        &clip,
        &scale
    );
}
