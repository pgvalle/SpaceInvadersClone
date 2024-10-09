#pragma once

#include "Entity.h"

struct Invader : Entity
{
  int type;
  int x, y;
  int clipIndex;

  Invader(int row, int col);

  Explosion *onHit(const SDL_Rect &rect) override;
  SDL_Rect getHitbox() const override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;

  void move(int xOff, int yOff);
};
