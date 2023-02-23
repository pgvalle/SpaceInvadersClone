#ifndef APP_H
#define APP_H

#include <SDL.h>

///////////////////////////////////////////////////////////////////////////////
// IMPORTANT MACROS //
///////////////////////////////////////////////////////////////////////////////

#define RESOURCES "./res"

#define FPS 60

#define SCALE 2 // Only to scale window size

#define TILE 8 // width and height of a tile
// These are the dimensions of the canvas. NOT of the window.
#define WIDTH  (28 * TILE) // 240
#define HEIGHT (32 * TILE) // 224

///////////////////////////////////////////////////////////////////////////////
// GLOBAL CONTEXT //
///////////////////////////////////////////////////////////////////////////////

extern enum screen_t {
    SCREEN_EXIT = 0,
    SCREEN_MENU,
    SCREEN_PLAY,
    SCREEN_PAUSE,
    SCREEN_GAMEOVER,
} screen;
extern SDL_Window* win;
extern SDL_Renderer* ren;

extern int credits;
extern int score, hi_score;

extern SDL_Texture* atlas;

///////////////////////////////////////////////////////////////////////////////
// UTILITIES //
///////////////////////////////////////////////////////////////////////////////

void render_clip(const SDL_Rect* clip, int x, int y);
void render_text(const char* text, int length, int x, int y);

int point_in_rect(const SDL_Point* point, const SDL_Rect* rect);


#endif // APP_H
