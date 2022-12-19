#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "entities/Cannon.h"

#define APP_TITLE "Space Invaders"

#define APP_SCALE_FACTOR 3
#define APP_SCALE(value) APP_SCALE_FACTOR * value

#define APP_WINDOW_WIDTH  APP_SCALE(224)
#define APP_WINDOW_HEIGHT APP_SCALE(256)

// ~~ 20 polls per frame
#define APP_EVENTPOLLING_DELTA 50

#define APP_ASSETS_BASEDIR "../../../res"

struct App
{
    enum AppState
    {
        MAINMENU = 0,
        GAMEPLAY = 1,
        GAMEOVER = 2
    } state;

    SDL_bool shouldQuit;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    SDL_bool hadEvent;
    Uint32 frameTime;

    SDL_Texture* textures[20];
};

struct App* App_Get();

#endif // APP_H_INCLUDED