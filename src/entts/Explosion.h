#pragma once

#include "common.h"
#include "util/Timer.h"

struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Timer lifespan;

  void onTick(float dt);
  void onUpdate(float dt);
  void onRender() const;
};
