#include "Application.h"
#include <string.h>
#include <ctype.h>

// Single instance
struct Application* app = NULL;
struct Application* GetAppInstance()
{
    return app;
}

#define CHARACTER_MAP \
    " A B C D E F G H  I J K L M N O P Q R S T U V W X Y Z 0  1 2 3 4 5 6 7 8 9 <  > "

int FindInCharacterMap(char c)
{
    static const char* characterMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789<> ";
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
    const int widthPace = 8;
    const int textLen = strlen(text);
    for (int i = 0; i < textLen; i++)
    {
        int indexMapping = FindInCharacterMap(text[i]);
        if (indexMapping != -1)
        {
            SDL_Rect clip = { indexMapping * widthPace, 0, widthPace, 8 };
            SDL_Rect scale = { 3 * x, 3 * y, 3 * widthPace, 3 * 8 };
            if (toupper(text[i]) == 'I' || text[i] == '1')
                scale.x -= 3;

            SDL_RenderCopy(app->renderer, app->uiTex, &clip, &scale);
        }

        x += widthPace;
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

    app->shouldClose = false;
    app->window = SDL_CreateWindow(APP_TITLE, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 3*224, 3*256, 0);
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
        TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "font.ttf", 8);
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
