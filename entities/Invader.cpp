#include "Horde.h"
#include "App.h"

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
  frame = !frame;
}

void Horde::Invader::render() const
{
  switch (type)
  {
    case 0:
      App::renderTile(x, y, 6 + frame);
      break;
    default:
    {
      const int base = 6 * (type + 1);
      App::renderTile(x, y, base + frame);
      App::setFlip(SDL_FLIP_HORIZONTAL);
      App::renderTile(x, y, base + frame);
      App::setFlip(SDL_FLIP_NONE);
      break;
    }
  }
}
