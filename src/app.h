#ifndef APP_H
#define APP_H

#include <SDL.h>

extern struct app_context_t {
    enum {
        SCREEN_EXIT = 0,
        SCREEN_MENU,
        SCREEN_PLAY,
        SCREEN_PAUSE,
        SCREEN_OVER,
    } screen;

    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Texture* atlas;

    // game logic stuff that have to be "global"

    int score, high_score;
    int credits;
} ctx;

static inline
void increase_score(int value)
{
  ctx.score += value;
  if (ctx.score > ctx.high_score) {
    ctx.high_score = ctx.score;
  }
}

static inline
void add_credit()
{
  if (ctx.credits < 99) {
    ctx.credits++;
  }
}

static inline
void remove_credit()
{
  if (ctx.credits > 0) {
    ctx.credits--;
  }
}

static inline
void render_clip(const SDL_Rect* clip, int x, int y)
{
    const SDL_Rect scale = { x, y, clip->w, clip->h };
    SDL_RenderCopy(ctx.ren, ctx.atlas, clip, &scale);
}

void render_text(const char* text, int len, int x, int y);

void render_score_advances_table();


#endif // APP_H
