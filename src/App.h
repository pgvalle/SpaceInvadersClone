#ifndef APP_H_INCLUDED
#define APP_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include "entities/Cannon.h"
#include "entities/Horde.h"

#define APP_TITLE "Space Invaders"

// To prevent stuff being too small
#define APP_SCALE_FACTOR 3
// Alias for scaling values
#define APP_SCALE(value) APP_SCALE_FACTOR * (value)

#define APP_WINDOW_WIDTH  APP_SCALE(224)
#define APP_WINDOW_HEIGHT APP_SCALE(256)

#define APP_ASSETS_BASEDIR "../../../res/"

struct App
{
    enum AppState
    {
        MAINMENU = 0,
        GAMEPLAY = 1,
        GAMEOVER = 2
    } state;

    struct Game
    {
        struct Cannon cannon;
        struct Horde horde;
    } game;

    SDL_bool shouldQuit;
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    Uint32 frameTime;

    SDL_Texture* textures[20];
    TTF_Font* font;
};

struct App* GetAppInstance();

#endif // APP_H_INCLUDED