#pragma once

#include "common.h"

struct Explosion;

struct Entity
{
  virtual Explosion *onHit(const SDL_Rect &rect) = 0;
  virtual SDL_Rect getHitbox() const = 0;

  virtual void onTick(float dt) = 0;
  virtual void onUpdate(float dt) = 0;
  virtual void onRender() const = 0;
};
