#include "Application.h"
#include <string.h>
#include <ctype.h>

// Single instance
struct Application* app = NULL;
struct Application* GetApp()
{
    return app;
}

#define CHARACTER_MAP \
    "A B C D E F G H  I J K L M N O P Q R S T U V W X Y Z 0  1 2 3 4 5 6 7 8 9 <  > * "

int FindInCharacterMap(char c)
{
    static const char* characterMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<>*";
    const int upperC = toupper(c);
    for (int i = 0; i < strlen(characterMap); i++)
    {
        if (characterMap[i] == upperC)
            return i;
    }
    return -1;
}

void RenderText(int x, int y, const char* text)
{
    const int factor = app->options.scale;
    const int textLen = strlen(text);
    for (int i = 0; i < textLen; i++)
    {
        int indexMapping = FindInCharacterMap(text[i]);
        if (indexMapping != -1)
        {
            SDL_Rect clip = { indexMapping * APP_FONT_PTSIZE, 0, APP_FONT_PTSIZE, APP_FONT_PTSIZE };
            SDL_Rect scale = { factor * x, factor * y, factor * APP_FONT_PTSIZE, factor * APP_FONT_PTSIZE };
            if (toupper(text[i]) == 'I' || text[i] == '1')
                scale.x -= factor;

            SDL_RenderCopy(app->renderer, app->uiTex, &clip, &scale);
        }

        x += APP_FONT_PTSIZE;
    }
}

void RenderInt(int x, int y, int value)
{
    // convert value to a string
    char valueStr[12];
    sprintf(valueStr, "%d", value);
    // render it
    RenderText(x, y, valueStr);
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

    // load font and creating font atlas
    {
        TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "font.ttf", APP_FONT_PTSIZE);
        SDL_Surface* uiSurface = TTF_RenderUTF8_Solid(font, CHARACTER_MAP,
            (SDL_Color){ 255, 255, 255, 255 });
        app->uiTex = SDL_CreateTextureFromSurface(app->renderer, uiSurface);
        SDL_FreeSurface(uiSurface);
        TTF_CloseFont(font);
    }
}

void DestroyApp()
{
    SDL_DestroyTexture(app->entitiesTex);
    SDL_DestroyTexture(app->uiTex);

    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
}

void InitMainMenuState();
void UpdateMainMenuState();
void RenderMainMenuState();

void InitTransitionState();
void RenderTransitionState();
void UpdateTransitionState();

SDL_CalculatePitch(Uint32 format, int width)
{
    Sint64 pitch;

    if (SDL_ISPIXELFORMAT_FOURCC(format) || SDL_BITSPERPIXEL(format) >= 8) {
        pitch = ((Sint64)width * SDL_BYTESPERPIXEL(format));
    }
    else {
        pitch = (((Sint64)width * SDL_BITSPERPIXEL(format)) + 7) / 8;
    }
    pitch = (pitch + 3) & ~3;   /* 4-byte aligning for speed */
    return pitch;
}

void AppMainLoop()
{
    Uint32 before = 0;
    int count = 0;
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
        if (count == 1000)
        {
            int w, h;
            SDL_GetWindowSize(app->window, &w, &h);
            Uint32 pixelFormat = SDL_GetWindowPixelFormat(app->window);
            int pitch = SDL_CalculatePitch(pixelFormat, w);
            void* pixels = malloc(w*h*pitch);
            SDL_RenderReadPixels(app->renderer, NULL, 0, pixels, pitch);
            SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, 0, pitch, pixelFormat);
            SDL_SaveBMP(surface, "../../../a.bmp");
            SDL_FreeSurface(surface);
        }


        app->frameTime = SDL_GetTicks() - before;
        before = SDL_GetTicks();
        if (count < 1010)
            count++;
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
