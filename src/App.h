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
#include "entities/Shots.h"

#define APP_TITLE "Space Invaders"

// To fix scaling of app elements
#define APP_SCALE_FACTOR 2
// Alias to scale values
#define APP_SCALE(value) APP_SCALE_FACTOR * (value)

#define APP_WINDOW_WIDTH  APP_SCALE(224)
#define APP_WINDOW_HEIGHT APP_SCALE(256)

#define APP_ASSETS_BASEDIR "../../../res/"


enum TextureIndex
{
    /* Image resources */

    TEXINDEX_CANNON,
    TEXINDEX_HORDE,
    TEXINDEX_SHIELD,
    TEXINDEX_TOURIST,

    /* UI(Text) items */

    // Special
    TEXINDEX_COUNT
};

struct App
{
    enum AppState
    {
        APPSTATE_MAINMENU,
        APPSTATE_GAMEPLAY,
        APPSTATE_GAMEOVER
    } state;

    struct Game
    {
        struct Cannon cannon;
        struct Horde horde;
    } game;

    bool shouldQuit;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Event event;

    Uint32 frameTime;

    /* Assets */

    SDL_Texture* textures[TEXINDEX_COUNT];
    TTF_Font* font;
};

struct App* GetAppInstance();

#endif // APP_H_INCLUDED