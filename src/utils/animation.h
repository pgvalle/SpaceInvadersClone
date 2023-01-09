#ifndef UTILS_ANIMATION_H
#define UTILS_ANIMATION_H

#include "timer.h"
#include <SDL2/SDL_rect.h>

typedef struct animation_frame_t
{
    SDL_Rect clip;
    t1mer_t timer;
} animation_frame_t;

typedef struct animation_t
{
    uint8_t texture_id;
    animation_frame_t* frames;
    uint8_t current;
} animation_t;

// Usage Ex:
// animation_init(
//     anim,
//     texture_id,
//     2
//     (animation_frame_t){
//         .clip = {0, 0, 16, 16},
//         .timer = ...
//     },
//     (animation_frame_t){
//         .clip = {0, 0, 16, 16},
//         .timer = ...
//     }
// });
void animation_init(animation_t* anim, uint8_t texture_id, uint8_t size, ...);
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


#endif // UTILS_ANIMATION_H
