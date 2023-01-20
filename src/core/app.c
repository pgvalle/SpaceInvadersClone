#include "app.h"
#include "internal.h"
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

struct app_t app;

void app_run();

int main(int argc, char** args)
{
    // initialization

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    app.window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        0,
        0,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS
    );
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    app.frame_time = 0;
    
    asset_man_init();
    fsm_init();

    app_run(); // execution

    // termination

    fsm_destroy();
    asset_man_destroy();

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    Mix_CloseAudio();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void app_run()
{
    uint32_t start = 0, wait_timeout = 1000 / APP_FPS;

    while (!app.should_close && !fsm_empty())
    {
        // beginning of frame. Get current time.
        start = SDL_GetTicks();

        // wait for event
        if (SDL_WaitEventTimeout(&app.event, wait_timeout))
        {
            fsm_process_event_for_current();
            if (app.event.type == SDL_QUIT)
                break;

            // calculate remaining time to wait next loop.
            const uint32_t processing_time = SDL_GetTicks() - start;
            // careful not to be value lower than zero. it's an unsigned int.
            wait_timeout = processing_time < wait_timeout ?
                (wait_timeout - processing_time) : 0;
        }
        else
        {
            fsm_update_current();
            fsm_render_current();
            fsm_update();

            app.frame_time = 0; // reset frame time
            wait_timeout = 1000 / APP_FPS; // reset event wait timeout
        }

        // accumulate frame time
        app.frame_time += SDL_GetTicks() - start;
    }
}
