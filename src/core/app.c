#include "app.h"
#include "fsm.h"
#include "asset_man.h"

#include <stdio.h>

#define APP_DEFAULT_SCALE 2
#define APP_DEFAULT_VOLUME 50

struct app_t app;

void app_init()
{
    app.fullscreen = false;
    
    // calculate actual scale in fullscreen
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    int hscale = mode.w / APP_VSCREEN_WIDTH;
    int vscale = mode.h / APP_VSCREEN_HEIGHT;
    // minimal scale factor to fit cool in fullscreen
    app.fs_scale = hscale < vscale ? hscale : vscale;

    app.scale = APP_DEFAULT_SCALE;
    app.volume = APP_DEFAULT_VOLUME;

    app.should_close = false;

    app.window = SDL_CreateWindow(
        "Space Invaders Clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app.scale * APP_VSCREEN_WIDTH,
        app.scale * APP_VSCREEN_HEIGHT,
        app.fullscreen ? SDL_WINDOW_FULLSCREEN : 0
    );
    
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    
    app.frame_time = 0;

    asset_man_init();

    fsm_init();
}

void app_destroy()
{
    // destroy all states properly because we're quitting the app.
    fsm_destroy();
    // destroy all resources
    asset_man_destroy();

    // app stuff
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}

void app_run()
{
    uint32_t before = SDL_GetTicks();

    while (!app.should_close && !fsm_empty())
    {
        // calculate frame deltatime
        app.frame_time = SDL_GetTicks() - before;
        if (app.frame_time < 16) // only finish frame if fps is around 60
            continue;

        // now a new frame started
        before = SDL_GetTicks();

        // poll for next event in queue
        SDL_PollEvent(&app.event);
        if (app.event.type == SDL_QUIT)
            app.should_close = true;

        fsm_update_current_state();
        fsm_render_current_state();

        fsm_update();
    }
}
