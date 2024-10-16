#include "SIC.h"

void SIC::init() {
  window = SDL_CreateWindow(
      "Space Invaders Clone",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      WIDTH,
      HEIGHT,
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
      {255, 255, 255, 255},
      TTF_STYLE_NORMAL));

  score = 0;
  high_score = 0; // TODO: implement loading high score from file
  
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
}

void SIC::quit() {
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void SIC::loop() {
  SDL_assert(screens.size() > 0);

  int screenid = 0;
  const Uint32 tpf = 1000 / FRAMERATE; // time per frame

  while (screenid != EXIT_HOOK) {
    const Uint32 start = SDL_GetTicks();
 
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    screens[screenid].draw();
    SDL_RenderPresent(renderer);

    const int new_screenid = screens[screenid].updt();

    if (new_screenid != KEEP_SCENE)
      screenid = new_screenid;
    
    if (score > high_score)
      high_score = score;

    const Uint32 delta = SDL_GetTicks() - start;

    if (delta < tpf)
      SDL_Delay(tpf - delta);
  }
}

void SIC::define_screen(Screen &&screen) {
  std::move(screen);
  screens.push_back(screen);
}

SIC *sic;
