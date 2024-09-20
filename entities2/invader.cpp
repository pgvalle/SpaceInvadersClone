#include "invader.h"
#include "globals.h"

Invader::Invader(int row, int col)
{
  type = 0;
  if (row > 2)
    type = 2;
  else if (row > 1)
    type = 1;

  animationFrame = 0;

  x = 2 + (3 + 2 * col) * TILE;
  y = 0 + (8 + 2 * row) * TILE;
}

int Invader::getScoreValue() const
{
  return 10 * (3 - type); // type 0: 30, type 1: 20, type 2: 10
}

SDL_Rect Invader::getRect() const
{
  if (type == 2)
    return {x - 2, y, 12, 8};
  if (type == 1)
    return {x - 1, y, 11, 8};

  return {x, y, 8, 8};
}

void Invader::move(int xOff, int yOff)
{
  x += xOff;
  y += yOff;
  // each movement should change animation frame
  animationFrame = !animationFrame;
}

void Invader::render() const
{
  const SDL_Rect dst = getRect();
  const SDL_Rect src = {animationFrame * dst.w, 16 + 8 * type, dst.w, dst.h};

  SDL_RenderCopy(ren, atlas, &src, &dst);
}