#pragma once

#include "common.h"
#include "Explosion.h"

struct Shot
{
  float x, y, dy;

  Explosion *collisionCallback(const SDL_Rect &expClip);
  SDL_Rect getCollider() const;
  void draw() const;
};
