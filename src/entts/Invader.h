#ifndef ENTITIES_INVADER_H
#define ENTITIES_INVADER_H

#include "common.h"
#include "Explosion.h"

struct Invader
{
  int type;
  int animationFrame;
  int x, y;

  Invader(int row, int col);

  Explosion *onHit(const SDL_Rect &rect) const;
  SDL_Rect getRect() const;
  void move(int xOff, int yOff);
  void render() const;
};

#endif // ENTITIES_INVADER_H