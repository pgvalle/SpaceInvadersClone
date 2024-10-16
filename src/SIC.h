#pragma once

#include "common.h"

#define KEEP_SCENE -1
#define EXIT_HOOK -2

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)
#define FRAMERATE 30

struct Screen {
  std::function<void()> init, draw;
  std::function<int()> updt;
};

struct SIC {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, high_score;
  std::vector<Screen> screens;

  void init();
  void quit();
  void loop();
  void define_screen(Screen &&screen);
};

extern SIC *sic;
