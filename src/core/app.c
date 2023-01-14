#include "app.h"
#include "internal.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <time.h>

struct app_t app;

void app_init();
void app_destroy();
void app_run();

int main(int argc, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    app_init();
    app_run();
    app_destroy();

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void app_init()
{
    app.fullscreen = false;
    app.scale = 0;
    app.volume = 0;
    app.should_close = false;
    app.window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0,
        0,
        SDL_WINDOW_HIDDEN | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS
    );
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    random_init(&app.random, 0xab803fc1);
    app.frame_time = 0;

    asset_man_init();

    // last thing to initialize so that state can access asset_man
    fsm_init();
}

void app_destroy()
{
    fsm_destroy();
    asset_man_destroy();

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
        if (app.frame_time < 17) // only finish frame if fps is around 60
            continue;

        // now a new frame started
        before = SDL_GetTicks();

        SDL_PollEvent(&app.event);

        fsm_update_current_state();
        fsm_render_current_state();

        fsm_update();
    }
}
