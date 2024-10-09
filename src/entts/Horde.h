#pragma once

#include "common.h"
#include "Invader.h"
#include "Explosion.h"

struct Horde : Entity
{
  std::vector<Invader> invaders;
  int x, vx;
  bool populating;
  Timer clock;

  Horde();

  Explosion *onHit(const SDL_Rect &rect) override;
  SDL_Rect getHitbox() const override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;
};
