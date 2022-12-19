#include "App.h"

// ========================================================================= //
// LIBRARIES SETUP
// ========================================================================= //

void OpenLibraries()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();
}

void CloseLibraries()
{
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

// API for entities to access app data

struct App app;
struct App* App_Get()
{
    return &app;
}

// ========================================================================= //
// ASSET MANAGEMENT
// ========================================================================= //

void LoadAssets()
{
    // base stuff
    app.state = GAMEPLAY;
    app.shouldQuit = SDL_FALSE;
    app.window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT, 0);
    app.renderer = SDL_CreateRenderer(app.window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    app.hadEvent = SDL_FALSE;
    app.frameTime = 0;

    // textures
    app.textures[CANNON_TEX_INDEX] = IMG_LoadTexture(app.renderer,
        APP_ASSETS_BASEDIR "img / cannon.png");
}

void FreeAssets()
{
    // textures
    SDL_DestroyTexture(app.textures[CANNON_TEX_INDEX]);

    // base stuff
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}

void MainLoop();

// ========================================================================= //
// ENTRY POINT
// ========================================================================= //

int main(int argc, const char** args)
{
    OpenLibraries();
    LoadAssets();

    MainLoop();

    FreeAssets();
    CloseLibraries();

    return 0;
}

// Update app

void App_PollEventsAndComputeFrameTime()
{
    // To calculate frametime
    static Uint32 before = 0;
    static Uint32 now = 0;
    // to know when to poll events
    static Uint32 eventTimer = 0;

    // calculating frametime
    before = now;
    now = SDL_GetTicks();
    app.frameTime = now - before;

    // updating event data
    eventTimer += app.frameTime;
    if (eventTimer < APP_EVENTPOLLING_DELTA)
        app.hadEvent = SDL_FALSE;
    else
    {
        app.hadEvent = SDL_PollEvent(&app.event);
        eventTimer -= APP_EVENTPOLLING_DELTA;
    }
}

// ========================================================================= //
// GAMEPLAY
// ========================================================================= //

void Gameplay_EventUpdate()
{

}

void Gameplay_TimeUpdate()
{

}

void Gameplay_Render()
{

}

void MainLoop()
{
    while (!app.shouldQuit)
    {
        App_PollEventsAndComputeFrameTime();

        switch (app.state)
        {
        case GAMEPLAY:
            if (app.hadEvent)
                Gameplay_EventUpdate();
            Gameplay_TimeUpdate();
            Gameplay_Render();
        }
    }
}