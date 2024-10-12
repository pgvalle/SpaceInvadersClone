#pragma once

#include "common.h"

struct Bunker
{
  SDL_Rect outerBounds;
  bool bits[352];

  Bunker();

  void draw() const;
};
