#include "App.h"

// Single instance
struct App* app = NULL;
struct App* GetAppInstance()
{
    return app;
}

void InitApp()
{
    // allocating memory for app struct
    app = malloc(sizeof(*app));

    // base stuff
    app->state = APPSTATE_GAMEPLAY;
    app->shouldQuit = false;
    app->window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT, 0);
    app->renderer = SDL_CreateRenderer(app->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    app->frameTime = 0;

    // icon
    {
        SDL_RWops* ops = SDL_RWFromFile(APP_ASSETS_BASEDIR "icon.svg", "rb");
        SDL_Surface* icon = IMG_LoadSVG_RW(ops);
        SDL_SetWindowIcon(app->window, icon);
        SDL_RWclose(ops);
        SDL_FreeSurface(icon);
    }

    // font
    app->font = TTF_OpenFont(APP_ASSETS_BASEDIR "font.ttf",
        APP_SCALE(10));

    // textures
    for (int i = 0; i < TEXINDEX_COUNT; i++)
        app->textures[i] = NULL;
    app->textures[TEXINDEX_CANNON] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/cannon.png");
    app->textures[TEXINDEX_HORDE] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/invaders.png");
    app->textures[TEXINDEX_TOURIST] = IMG_LoadTexture(app->renderer,
        APP_ASSETS_BASEDIR "img/tourist.png");

    app->game.tourist.spawned = false;
    app->game.tourist.deathTimer = 0;
    app->game.tourist.spawnTimer = 0;
    app->game.tourist.spawnTimeout = 5000;
}

void DestroyApp()
{
    // textures
    for (int i = 0; i < TEXINDEX_COUNT; i++)
    {
        if (app->textures[i])
            SDL_DestroyTexture(app->textures[i]);
    }

    // font
    TTF_CloseFont(app->font);

    // base stuff
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    free(app);
}

// states routines declaration

void UpdateMainMenuState();
void RenderMainMenuState();

void UpdateGameplayState();
void RenderGameplayState();

void UpdateGameOverState();
void RenderGameOverState();

void MainLoop()
{
    Uint32 before = 0;

    while (!app->shouldQuit)
    {
        // polling events
        SDL_PollEvent(&app->event);
        // just to make sure the user can quit the app
        if (app->event.type == SDL_QUIT)
            app->shouldQuit = SDL_TRUE;

        switch (app->state)
        {
        case APPSTATE_MAINMENU:
            UpdateMainMenuState();
            RenderMainMenuState();
            break;
        case APPSTATE_GAMEPLAY:
            UpdateGameplayState();
            RenderGameplayState();
            break;
        case APPSTATE_GAMEOVER:
            UpdateGameOverState();
            RenderGameOverState();
            break;
        }

        // updating frame delta time
        app->frameTime = SDL_GetTicks() - before;
        before = SDL_GetTicks();
    }
}

void RunApp()
{
    // Initializing libraries

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();

    // Initialize app and all of its resources
    InitApp();

    MainLoop();

    // properly free everything
    DestroyApp();

    // Quitting libraries

    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}
