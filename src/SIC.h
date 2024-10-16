#pragma once

#include <cstdio>
#include <cmath>
#include <ctime>
#include <vector>
#include <string>
#include <functional>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>


#define FRAMERATE 30
#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

#define KEEP_SCREEN -1
#define EXIT_HOOK -2


struct SIC {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, high_score;

  struct Screen {
    std::function<void()> init, draw;
    std::function< int()> updt;
  };
  std::vector<Screen> screens;

  void init();
  void quit();
  void loop();
  void define_screen(Screen &&screen);
};

extern SIC *sic;

