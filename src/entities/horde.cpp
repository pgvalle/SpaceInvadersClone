#include "horde.h"
#include "globals.h"

Invader::Invader(int row, int col)
{
  type = 0;
  if (row > 2)
    type = 2;
  else if (row > 0)
    type = 1;

  animationFrame = 0;

  x = 2 + (3 + 2 * col) * TILE;
  y = 0 + (8 + 2 * row) * TILE;
}

int Invader::getScoreValue() const
{
  return 10 * (3 - type); // 30, 20, 10
}

SDL_Rect Invader::getRect() const
{
  SDL_Rect rect = {x + 2, y, 8, 8};
  if (type == 2)
    rect = {x + 1, y, 11, 8};
  else if (type == 1)
    rect = {x, y, 12, 8};

  return rect;
}

void Invader::move(int xOff, int yOff)
{
  x += xOff;
  y += yOff;
  // each movement should change animation frame
  animationFrame = !animationFrame;
}
