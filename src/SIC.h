#pragma once

#include "common.h"

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

#define TICKRATE 20

struct SIC
{
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, highScore;
  bool shouldStop;

  SIC();
  ~SIC();

  void add2Score(int val);
  void loop();
  void onTick(float dt);
  void onUpdate(float dt, const SDL_Event &event);
  void onRender() const;
};

extern SIC *g;

