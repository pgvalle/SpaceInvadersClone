#ifndef APP_H
#define APP_H

#include <stdint.h>
#include <stdbool.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define APP_FONT_PTSIZE 8

#define APP_VSCREEN_WIDTH  224
#define APP_VSCREEN_HEIGHT 256

#define APP_CHARACTER_SET "A B C D E F G H  I J K L M N O P Q R S T "\
    "U V W X Y Z 0  1 2 3 4 5 6 7 8 9 <  > * ? -  = "

extern struct app_t {
    // some app settings
    bool fullscreen;
    int fs_scale;
    int scale;
    int volume;

    bool should_close;

    SDL_Window* window;
    SDL_Event event;

    SDL_Renderer* renderer;

    uint32_t frame_time;
} app; // single instance

void app_init();
void app_destroy();
void app_run();

#endif // APP_H
