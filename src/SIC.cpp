#include "SIC.h"
#include <stdarg.h>

void SIC::init() {
  window = SDL_CreateWindow(
      "Space Invaders Clone",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      2 * WIDTH,
      2 * HEIGHT,
      SDL_WINDOW_RESIZABLE);
  SDL_assert(window);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_assert(renderer);

  atlas = IMG_LoadTexture(renderer, "res/atlas.png");
  SDL_assert(atlas);

  font = FC_CreateFont();
  SDL_assert(FC_LoadFont(
      font,
      renderer,
      "res/ps2p.ttf",
      TILE,
      { 255, 255, 255, 255 },
      TTF_STYLE_NORMAL));

  score = 0;
  high_score = 0; // TODO: implement loading high score from file
  credits = 0;

  screens[0].init = menu_init;
  screens[0].draw = menu_draw;
  screens[0].update = menu_update;
  
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
  FC_SetDefaultColor(font, { 255, 255, 255, 255 });
}

void SIC::quit() {
  FC_FreeFont(font);
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void SIC::loop() {
  Uint32 tpf = 1000 / FPS;
  int screen = -1, new_screen = 0;

  while (new_screen != SCREEN_EXIT_HOOK) {
    Uint32 start = SDL_GetTicks();

    if (new_screen != SCREEN_UNCHANGED) {
      screen = new_screen;
      screens[screen].init();
    }

    SDL_Event event;
    SDL_PollEvent(&event);
    new_screen = screens[screen].update(event);

    high_score = std::max(score, high_score);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    screens[screen].draw();
    SDL_RenderPresent(renderer);

    Uint32 delta = SDL_GetTicks() - start;

    if (delta < tpf)
      SDL_Delay(tpf - delta);
  }
}

void SIC::render_text(int x, int y, const char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  FC_Draw(font, renderer, x, y, fmt, args);
  va_end(args);
}

void SIC::render_clip(const SDL_Rect &src, const SDL_Rect &dst) {
  SDL_RenderCopy(renderer, atlas, &src, &dst);
}

struct SIC *_sic;

