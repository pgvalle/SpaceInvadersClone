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
#include "entities/Tourist.h"

#define APP_TITLE "Space Invaders"

// To fix scaling of app elements
#define APP_SCALE_FACTOR 2
// Alias to scale values
#define APP_SCALE(value) APP_SCALE_FACTOR * (value)

#define APP_VSCREEN_WIDTH  224 // width in game units
#define APP_VSCREEN_HEIGHT 256 // height in game units
#define APP_SCREEN_WIDTH  APP_SCALE(APP_VSCREEN_WIDTH)  // real width
#define APP_SCREEN_HEIGHT APP_SCALE(APP_VSCREEN_HEIGHT) // real height

#define APP_ASSETS_BASEDIR "../../../res/"

#define APP_GET_KEYS const Uint8* keys = SDL_GetKeyboardState(NULL)

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
        struct Tourist tourist;
    } game;

    bool shouldQuit;

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Event event;

    Uint32 frameTime;

    // Assets

    SDL_Texture* textures[TEXINDEX_COUNT];
    TTF_Font* font;
};

struct App* GetAppInstance();

#endif // APP_H_INCLUDED
