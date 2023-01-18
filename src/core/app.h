#ifndef CORE_APP_H
#define CORE_APP_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "utils/random.h"

#define APP_FPS 60
#define APP_SCALE 2

extern struct app_t
{
    bool should_close;

    SDL_Window* window;
    SDL_Event event;

    SDL_Renderer* renderer;

    uint32_t frame_time;
} app; // single instance


#endif // CORE_APP_H
