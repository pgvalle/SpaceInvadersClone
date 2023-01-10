#ifndef UTILS_RENDER_H
#define UTILS_RENDER_H

#include <stdint.h>
#include <SDL_rect.h>

// x and y are in-game coordinates.
void clip_render(
    const SDL_Rect* clip, uint8_t texture, int world_w, int world_h, int x, int y
);


#endif // UTILS_RENDER_H
