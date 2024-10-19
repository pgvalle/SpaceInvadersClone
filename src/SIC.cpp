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
      "res/font.ttf",
      TILE,
      { 255, 255, 255, 255 },
      TTF_STYLE_NORMAL));

  score = 0;
  high_score = 0; // TODO: implement loading high score from file
  lives = 3;
  credits = 0;

  screens.push_back({ splash_init, splash_draw, splash_update });
  screens.push_back({ ready_init, ready_draw, ready_update });
  screens.push_back({ play_init, play_draw, play_update });

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
  Uint32 tpf = 1000 / FPS; // time per frame
  int si = -1, new_si = 0; // screen index and new screen index

  while (new_si != NULL_SCREEN) {
    SDL_Event event;
    Uint32 start = SDL_GetTicks(), delta = 0;

    // if screen changed, initialize screen state
    if (si != new_si) {
      si = new_si;
      screens[si].init();
    }

    // event processing
    SDL_PollEvent(&event);
    new_si = screens[si].update(event);

    high_score = std::max(score, high_score);

    // rendering
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    screens[si].draw();
    SDL_RenderPresent(renderer);

    // fps controlling
    delta = SDL_GetTicks() - start;
    if (delta < tpf)
      SDL_Delay(tpf - delta);
  }
}

void SIC::draw_text(int x, int y, const char *fmt, ...) {
  static char buffer[128];
  va_list args;

  va_start(args, fmt);
  vsprintf(buffer, fmt, args);
  va_end(args);

  FC_Draw(font, renderer, x, y, buffer);
}

void SIC::draw_clip(const SDL_Rect &src, const SDL_Rect &dst) {
  SDL_RenderCopy(renderer, atlas, &src, &dst);
}

struct SIC sic;

