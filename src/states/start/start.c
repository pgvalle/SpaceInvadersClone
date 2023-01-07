#include "../../core/app.h"
#include "../../core/fsm.h"

void start_state_init()
{

}

void start_state_destroy()
{

}

void start_state_update()
{
    if (app.event.type == SDL_KEYUP &&
    app.event.key.keysym.sym == SDLK_RETURN &&
    app.event.key.keysym.mod & KMOD_ALT)
    {
        app.fullscreen = !app.fullscreen;
        SDL_SetWindowFullscreen(app.window, app.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
    }
}

void start_state_render()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    int scale = app.fullscreen ? app.fs_scale : app.scale;
    int w, h;
    SDL_GetWindowSize(app.window, &w, &h);
    int x_off_fs = (w - app.fs_scale * APP_VSCREEN_WIDTH) / 2;
    int y_off_fs = (h - app.fs_scale * APP_VSCREEN_HEIGHT) / 2;
    SDL_Rect rect = {
        app.fullscreen ? x_off_fs : 0,
        app.fullscreen ? y_off_fs : 0,
        scale * 100,
        scale * 100
    };
    SDL_RenderFillRect(app.renderer, &rect);
    SDL_RenderPresent(app.renderer);
}