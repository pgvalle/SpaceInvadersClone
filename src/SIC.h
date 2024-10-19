#pragma once

#include <cstdio>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_FontCache.h>

#include "screens.h"

#define FPS 60
#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

struct SIC {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, high_score, lives, credits;
  std::vector<Screen> screens;

  void init();
  void quit();
  void loop();
  void draw_text(int x, int y, const char *fmt, ...);
  void draw_clip(const SDL_Rect &src, const SDL_Rect &dst);
};

extern SIC sic;

