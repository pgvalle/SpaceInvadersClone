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
#define APP_RESOURCE_DIR "../res/"
#define APP_FONT_PTSIZE 8
#define APP_CHARACTER_SET "A B C D E F G H  I J K L M N O P Q R S T "\
    "U V W X Y Z 0  1 2 3 4 5 6 7 8 9 <  > * ? "

struct Application
{
    struct AppOptions
    {
        int volume;
        int scale;
    } options;

    bool shouldClose;

    SDL_Window* window;
    SDL_Event event;

    SDL_Renderer* renderer;

    Uint32 frameTime;

    // Assets
    SDL_Texture* entitiesTex, * charsTex;
};

// single instance
extern struct Application app;

#endif // APPLICATION_H
