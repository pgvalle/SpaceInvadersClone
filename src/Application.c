#include "Application.h"
#include "states/StateMachine.h"

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

void LoadTextures()
{
    app.entitiesTex = IMG_LoadTexture(app.renderer, APP_RESOURCE_DIR "atlas.png");

    // The character map
    // It's spaced in a certain way so that the text rendered is monospaced
    const char* characterMapMono = APP_CHARACTER_SET;

    TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "font.ttf", APP_FONT_PTSIZE);

    // render all character set in different surfaces separated by color
    SDL_Surface* whiteCharsSurf = TTF_RenderUTF8_Solid(
        font,
        characterMapMono,
        (SDL_Color){ 255, 255, 255, 255 }
    );
    SDL_Surface* redCharsSurf = TTF_RenderUTF8_Solid(
        font,
        characterMapMono,
        (SDL_Color){ 0xD8, 0x20, 0x20, 255 }
    );

    // Create temporary surface to merge everything together
    SDL_Surface* allCharsSurf = SDL_CreateRGBSurfaceWithFormat(
        0,
        whiteCharsSurf->w,
        2 * APP_FONT_PTSIZE,
        0,
        SDL_PIXELFORMAT_RGB888
    );

    // the surfaces to be copied have some unused gaps
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
    app.charsTex = SDL_CreateTextureFromSurface(app.renderer, allCharsSurf);
    SDL_FreeSurface(allCharsSurf);

    TTF_CloseFont(font);
}

void InitApp()
{
    app.options.volume = 5;
    app.options.scale = 2;
    app.shouldClose = false;
    app.window = SDL_CreateWindow(
        APP_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app.options.scale * 224,
        app.options.scale * 256,
        0
    );
    app.renderer = SDL_CreateRenderer(
        app.window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    app.frameTime = 0;

    // loading window icon
    {
        SDL_RWops* ops = SDL_RWFromFile(APP_RESOURCE_DIR "icon.svg", "rb");
        SDL_Surface* icon = IMG_LoadSVG_RW(ops);
        SDL_SetWindowIcon(app.window, icon);
        SDL_RWclose(ops);
        SDL_FreeSurface(icon);
    }

    LoadTextures();

    InitStateMachine();
}

void DestroyApp()
{
    // destroy all states properly because we're quitting the app.
    DestroyStateMachine();

    SDL_DestroyTexture(app.entitiesTex);
    SDL_DestroyTexture(app.charsTex);

    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
}

void RunApp()
{
    Uint32 before = 0;

    while (!app.shouldClose)
    {
        // calculate last frame time
        app.frameTime = SDL_GetTicks() - before;
        before = SDL_GetTicks();

        // reset event and poll for next in queue
        app.event.type = SDL_FIRSTEVENT;
        SDL_PollEvent(&app.event);
        if (app.event.type == SDL_QUIT)
            app.shouldClose = true;

        UpdateCurrentState();
        RenderCurrentState();

        UpdateStateChanges();
    }
}
