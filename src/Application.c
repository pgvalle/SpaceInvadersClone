#include "Application.h"
#include "States/StateMachine.h"

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

void LoadEntitiesTexture()
{
    bool entitiesTexValid = true;

    app.atlasTex = IMG_LoadTexture(app.renderer, APP_RESOURCE_DIR "atlas.png");
    if (app.atlasTex == NULL)
    {
        SDL_LogError(0, "res/atlas.png couldn't be opened.\n");
        entitiesTexValid = false;
    }
    else
    {
        int width, height;
        SDL_QueryTexture(app.atlasTex, NULL, NULL, &width, &height);
        if (width < 48 || height < 48)
        {
            SDL_LogError(0, "res/atlas.png is %dx%d pixels.\n", width, height);
            fprintf(stderr, "It should be at least 48x48.\n");

            entitiesTexValid = false;
            SDL_DestroyTexture(app.atlasTex);
            app.atlasTex = NULL;
        }
    }

    if (!entitiesTexValid)
    {
        SDL_Log("Game entities will be rendered as white rectangles.\n");

        // Create temporary fallback surface
        SDL_Surface* fallbackSurf = SDL_CreateRGBSurfaceWithFormat(
            0, 48, 48, 0, SDL_PIXELFORMAT_RGB888
        );

        // paint it all white
        SDL_FillRect(fallbackSurf, NULL, 0xffffffff);

        // create fallback texture and free surface
        app.atlasTex = SDL_CreateTextureFromSurface(app.renderer, fallbackSurf);
        SDL_FreeSurface(fallbackSurf);
    }
}

void CreateCharsTexture()
{
    TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "font.ttf", APP_FONT_PTSIZE);
    if (font == NULL)
    {
        SDL_LogError(0, "res/font.ttf couldn't be opened.\n");
        SDL_Log("Text will be rendered as white rectangles.\n");

        // Create temporary fallback surface
        SDL_Surface* fallbackSurf = SDL_CreateRGBSurfaceWithFormat(
            0, 50 * APP_FONT_PTSIZE, 2 * APP_FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888
        );

        // paint it all white
        SDL_FillRect(fallbackSurf, NULL, 0xffffffff);

        // create fallback texture and free surface
        app.fontTex = SDL_CreateTextureFromSurface(app.renderer, fallbackSurf);
        SDL_FreeSurface(fallbackSurf);
    }
    else
    {
        // render all character set in different surfaces separated by color
        SDL_Surface* whiteCharsSurf = TTF_RenderUTF8_Solid(
            font, APP_CHARACTER_SET, (SDL_Color){ 255, 255, 255, 255 }
        );
        SDL_Surface* redCharsSurf = TTF_RenderUTF8_Solid(
            font, APP_CHARACTER_SET, (SDL_Color){ 216, 32, 32, 255 }
        );

        // Create temporary surface to merge everything together
        SDL_Surface* allCharsSurf = SDL_CreateRGBSurfaceWithFormat(
            0, whiteCharsSurf->w, 2 * APP_FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888
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
        app.fontTex = SDL_CreateTextureFromSurface(app.renderer, allCharsSurf);
        SDL_FreeSurface(allCharsSurf);

        TTF_CloseFont(font);
    }
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
    app.renderer = SDL_CreateRenderer(
        app.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    app.frameTime = 0;

    // loading window icon
    {
        SDL_RWops* ops = SDL_RWFromFile(APP_RESOURCE_DIR "icon.svg", "rb");
        if (ops == NULL)
        {
            SDL_LogWarn(0, "res/icon.svg couldn't be opened.\n");
            SDL_Log("The window icon is undefined.\n");
        }
        else
        {
            SDL_Surface* iconSurf = IMG_LoadSVG_RW(ops);
            SDL_SetWindowIcon(app.window, iconSurf);
            SDL_RWclose(ops);
            SDL_FreeSurface(iconSurf);
        }
    }

    LoadEntitiesTexture();
    CreateCharsTexture();

    InitStateMachine();
}

void DestroyApp()
{
    // destroy all states properly because we're quitting the app.
    DestroyStateMachine();

    SDL_DestroyTexture(app.atlasTex);
    SDL_DestroyTexture(app.fontTex);

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

        UpdateStateMachine();
    }
}
