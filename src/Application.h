#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define APP_TITLE "Space Invaders"

#define APP_RESOURCE_DIR "../../../res/"

struct Application
{
    bool shouldClose;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Event event;

    Uint32 frameTime;

    SDL_Texture* entitiesTex;
    SDL_Texture* uiTex;

    struct Options
    {
        int volume;
        int scale;
    };
};

struct Application* GetAppInstance();
// Non-alphanumeric characters are rendered as whitespaces, whitespaces included.
void RenderText(int x, int y, const char* text);
void RenderInt(int x, int y, int value);

#endif // APPLICATION_H