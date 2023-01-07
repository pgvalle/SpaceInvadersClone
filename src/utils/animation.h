#ifndef ANIMATION_H
#define ANIMATION_H

#include "tymer.h"
#include "stb_ds.h"
#include <SDL_rect.h>

typedef struct animation_frame_t {
    SDL_Rect rect;
    tymer_t timer;
} animation_frame_t;

typedef struct animation_t {
    uint16_t texture_id;
    animation_frame_t* frames;
    uint8_t current;
} animation_t;

// Using stdarg here. Really cool syntax sugar to create animations
void animation_init(animation_t* anim, uint16_t texture_id, uint8_t size, ...);
// Frees anim->frames and reset other fields
void animation_free(animation_t* anim);

static inline
void animation_reset(animation_t* anim)
{
    anim->current = 0;
}

bool animation_is_over(const animation_t* anim);

void animation_update(animation_t* anim);
void animation_render(const animation_t* anim, int x, int y);


#endif // ANIMATION_H