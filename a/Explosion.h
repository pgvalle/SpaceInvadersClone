#pragma once

#include "common.h"
#include "util/Ticker.h"

struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Ticker lifespan;

  void draw() const;
};
