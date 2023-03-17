#ifndef APP_H
#define APP_H

///////////////////////////////////////////////////////////////////////////////
// IMPORTANT MACROS //
///////////////////////////////////////////////////////////////////////////////

#define TILE 8
// These are the dimensions of the canvas. NOT of the window.
#define WIDTH  (28 * TILE) // 240
#define HEIGHT (32 * TILE) // 224

///////////////////////////////////////////////////////////////////////////////
// STATE //
///////////////////////////////////////////////////////////////////////////////

#include <SDL.h>

extern enum screen_t {
    SCREEN_EXIT = 0,
    SCREEN_MENU,
    SCREEN_PLAY,
    SCREEN_PAUSE,
    SCREEN_GAMEOVER,
} screen;
extern SDL_Window* win;
extern SDL_Renderer* ren;

extern SDL_Texture* atlas;

///////////////////////////////////////////////////////////////////////////////
// UTILS //
///////////////////////////////////////////////////////////////////////////////

static inline
void render_clip(const SDL_Rect* clip, int x, int y)
{
    const SDL_Rect scale = { x, y, clip->w, clip->h };
    SDL_RenderCopy(ren, atlas, clip, &scale);
}

void render_text(const char* text, int len, int x, int y);

static inline
int point_in_rect(const SDL_Point* point, const SDL_Rect* rect)
{
    return !(point->x < rect->x || point->x >= rect->x + rect->w ||
        point->y < rect->y || point->y >= rect->y + rect->h);
}

///////////////////////////////////////////////////////////////////////////////
// IMPORTANT APP FUNCTIONS //
///////////////////////////////////////////////////////////////////////////////

void load_app_resources();
void free_app_resources();

void process_app_events(const SDL_Event* event);
void update_app(Uint32 delta);
void render_app();

void run_app_loop();

#endif // APP_H
