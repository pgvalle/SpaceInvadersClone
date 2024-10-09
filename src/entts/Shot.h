#pragma once

#include "common.h"
#include "Explosion.h"

struct Shot
{
  float x, y, vy;
  SDL_Rect clip, expClip;

  Explosion *onHit();
  SDL_Rect getHitbox() const;
  void onUpdate(float dt);
  void onRender() const;
};
