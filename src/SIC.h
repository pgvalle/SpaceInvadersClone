#pragma once

#include <cstdio>
#include <cmath>
#include <ctime>
#include <string>

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
  Screen screens[MAX_SCREENS];

  void init();
  void quit();
  void loop();
  void render_text(int x, int y, const char *fmt, ...);
  void render_clip(const SDL_Rect &src, const SDL_Rect &dst);
};

extern SIC sic;

