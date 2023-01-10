#ifndef CORE_APP_H
#define CORE_APP_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

extern struct app_t {
    // some basic app settings
    bool fullscreen;
    int scale;
    int volume;

    bool should_close;

    SDL_Window* window;
    SDL_Event event;

    SDL_Renderer* renderer;

    uint32_t frame_time;
} app; // single instance


#endif // CORE_APP_H
