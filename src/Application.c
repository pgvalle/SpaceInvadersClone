#include "Application.h"
#include "States/StateMachine.h"
#include "Utils/Render.h"

#include <stdio.h>

struct Application app;

void InitApp();
void DestroyApp();
void RunApp();

int main(int argc, char** args)
{
    // Open libraries
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();

    // Initialize app instance and all of its resources
    InitApp();

    RunApp(); // Main loop

    // properly free everything
    DestroyApp();

    // Close libraries
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void InitApp()
{
    app.options.volume = 5;
    app.options.scale = 2;
    app.shouldClose = false;
    app.window = SDL_CreateWindow(
        "Space Invaders Clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app.options.scale * APP_VSCREEN_WIDTH,
        app.options.scale * APP_VSCREEN_HEIGHT,
        0
    );
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    
    app.frameTime = 0;

    LoadTextures();

    InitStateMachine();
}

void DestroyApp()
{
    // destroy all states properly because we're quitting the app.
    DestroyStateMachine();
    
    DestroyTextures();

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}

void RunApp()
{
    Uint32 before = SDL_GetTicks();

    while (!app.shouldClose)
    {
        // calculate frame deltatime
        app.frameTime = SDL_GetTicks() - before;
        if (app.frameTime < 16) // only finish frame if fps is around 60
            continue;

        // now a new frame started
        before = SDL_GetTicks();

        // poll for next event in queue
        SDL_PollEvent(&app.event);
        if (app.event.type == SDL_QUIT)
            app.shouldClose = true;

        UpdateCurrentState();
        RenderCurrentState();

        UpdateStateMachine();
    }
}
