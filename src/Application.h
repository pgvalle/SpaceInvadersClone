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

#define APP_FONT_PTSIZE 8

#define APP_RESOURCE_DIR "./res/"

struct Application
{
    enum AppState
    {
        APPSTATE_NONE,
        APPSTATE_MENU,
        APPSTATE_GAME,
        APPSTATE_OVER
    } currentState, nextState;

    struct AppOptions
    {
        int volume;
        int scale;
    } options;

    bool shouldClose;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Event event;

    Uint32 frameTime;

    // Assets

    SDL_Texture* entitiesTex;
    SDL_Texture* charsTex;
};

struct Application* GetApp();
// Non-alphanumeric characters are rendered as whitespaces
void RenderText(int x, int y, const char* text, bool red);
void RenderInt(int x, int y, int value, bool red);

#endif // APPLICATION_H
