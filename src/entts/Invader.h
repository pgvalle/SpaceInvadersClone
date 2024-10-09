#pragma once

#include "common.h"
#include "Explosion.h"

struct Invader
{
  int type;
  int x, y;
  int clipIndex;

  Invader(int row, int col);

  Explosion *onHit();
  SDL_Rect getHitbox() const;
  void onRender() const;
  void move(int xOff, int yOff);
};
