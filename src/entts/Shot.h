#pragma once

#include "common.h"

struct Shot
{
  int x, y, vy;
  SDL_Rect clip;

  void onUpdate(float dt);
  void onRender() const;
};
