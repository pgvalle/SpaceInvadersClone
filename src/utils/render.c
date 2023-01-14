#include "render.h"
#include "core/app.h"
#include "core/asset_man.h"

void clip_render(
    const SDL_Rect* clip, uint8_t texture, int world_w, int world_h, int x, int y
)
{
    int offset_x = 0, offset_y = 0;
    // to centralize canvas when fullscreen
    if (app.fullscreen)
    {
        int window_w, window_h;
        SDL_GetWindowSize(app.window, &window_w, &window_h);
        offset_x = (app.scale * world_w - window_w) / 2;
        offset_y = (app.scale * world_h - window_h) / 2;
    }

    const SDL_Rect scale = {
        app.scale * x + offset_x,
        app.scale * y + offset_y,
        app.scale * clip->w,
        app.scale * clip->h
    };

    SDL_RenderCopy(
        app.renderer, asset_man_get(ASSETTYPE_TEXTURE, texture), clip, &scale
    );
}
