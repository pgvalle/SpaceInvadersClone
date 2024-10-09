#pragma once

#include "util/Timer.h"
#include "Entity.h"

struct Explosion : Entity
{
  int x, y;
  SDL_Rect clip;
  Timer lifespan;

  Explosion *onHit(const SDL_Rect &rect) override;
  SDL_Rect getHitbox() const override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;
};
