#include "Application.h"

// Single instance
struct Application* app = NULL;
struct Application* GetApp()
{
    return app;
}

void LoadChars()
{
    // The character map
    // It's spaced in a certain way so that the texture generated is symmetric
    const char* characterMapMono =
        "A B C D E F G H  I J K L M N O P Q R S T "
        "U V W X Y Z 0  1 2 3 4 5 6 7 8 9 <  > * ";

    TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "font.ttf", APP_FONT_PTSIZE);

    // generate all the characters in different surfaces separated by color
    SDL_Surface* whiteCharsSurf = TTF_RenderUTF8_Solid(font, characterMapMono,
        (SDL_Color){ 255, 255, 255, 255 });
    SDL_Surface* redCharsSurf = TTF_RenderUTF8_Solid(font, characterMapMono,
        (SDL_Color){ 0xD8, 0x20, 0x20, 255 });

    // Create temporary surface to paste all colored characters to
    SDL_Surface* allCharsSurf = SDL_CreateRGBSurfaceWithFormat(0, whiteCharsSurf->w,
        2 * APP_FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888);

    // blit and free
    const SDL_Rect srcRect = { 0, 0,  whiteCharsSurf->w, APP_FONT_PTSIZE };
    // white goes above
    SDL_Rect destRect = { 0, 0, 0, 0 };
    SDL_BlitSurface(whiteCharsSurf, &srcRect, allCharsSurf, &destRect);
    SDL_FreeSurface(whiteCharsSurf);
    // red goes down
    destRect.y += APP_FONT_PTSIZE;
    SDL_BlitSurface(redCharsSurf, &srcRect, allCharsSurf, &destRect);
    SDL_FreeSurface(redCharsSurf);

    // finally create the texture and free the surface
    app->charsTex = SDL_CreateTextureFromSurface(app->renderer, allCharsSurf);
    SDL_FreeSurface(allCharsSurf);

    TTF_CloseFont(font);
}

void InitApp()
{
    app = malloc(sizeof(*app));

    app->options.volume = 5;
    app->options.scale = 2;
    app->shouldClose = false;
    app->window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, app->options.scale * 224, app->options.scale * 256, 0);
    app->renderer = SDL_CreateRenderer(app->window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    app->frameTime = 0;

    // loading window icon
    {
        SDL_RWops* ops = SDL_RWFromFile(APP_RESOURCE_DIR "icon.svg", "rb");
        SDL_Surface* icon = IMG_LoadSVG_RW(ops);
        SDL_SetWindowIcon(app->window, icon);
        SDL_RWclose(ops);
        SDL_FreeSurface(icon);
    }

    app->entitiesTex = IMG_LoadTexture(app->renderer,
        APP_RESOURCE_DIR "atlas.png");

    LoadChars();
}

void DestroyApp()
{
    SDL_DestroyTexture(app->entitiesTex);
    SDL_DestroyTexture(app->charsTex);

    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}

void InitMainMenuState();
void UpdateMainMenuState();
void RenderMainMenuState();

void AppMainLoop()
{
    Uint32 before = 0;
    InitMainMenuState();

    while (!app->shouldClose)
    {
        SDL_PollEvent(&app->event);
        if (app->event.type == SDL_QUIT)
            app->shouldClose = true;

        SDL_RenderClear(app->renderer);
        UpdateMainMenuState();
        RenderMainMenuState();
        SDL_RenderPresent(app->renderer);

        app->frameTime = SDL_GetTicks() - before;
        before = SDL_GetTicks();
    }
}

void RunApp()
{
    // Initialize libraries

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    TTF_Init();

    // Initialize app and all of its resources
    InitApp();

    AppMainLoop();

    // properly free everything
    DestroyApp();

    // Quit libraries
    TTF_Quit();
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}
