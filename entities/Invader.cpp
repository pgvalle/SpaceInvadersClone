#include "Horde.h"
#include "app/App.h"
#include "defines.h"


Horde::Invader::Invader(int col, int row)
{
  // invaders have different shapes depending on their starting row
  type = 0;
  if (row > 2) type = 2;
  else if (row > 0) type = 1;

  frame = 0;

  x = 2 + (3 + 2 * col) * TILE;
  y = (8 + 2 * row) * TILE;
}

SDL_Rect Horde::Invader::getHitbox() const
{
  SDL_Rect hitbox = {x + 2, y, 8, 8};
  if (type == 2) hitbox = {x + 1, y, 11, 8};
  else if (type == 1) hitbox = {x, y, 12, 8};

  return hitbox;
}

void Horde::Invader::move(int xOff, int yOff)
{
  x += xOff;
  y += yOff;
  // each movement should change animation frame
  frame = (frame + 1) % 2;
}

void Horde::Invader::render() const
{
  SDL_Rect clip = {12 * frame, 16, 12, 8};
  if (type == 2) clip.y = 32;
  else if (type == 1) clip.y = 24;

  renderClip(x, y, clip);
}
