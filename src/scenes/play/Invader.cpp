#include "app/App.h"
#include "Invader.h"
#include "defines.h"


Invader::Invader(int col, int row)
{
  // invaders have different shapes depending on their starting row
  type = 0;
  if (row > 2) type = 2;
  else if (row > 0) type = 1;

  frame = 0;

  x = 2 + (3 + 2 * col) * TILE;
  y = (8 + 2 * row) * TILE;
}

void Invader::move(int xOff, int yOff)
{
  x += xOff;
  y += yOff;
  // each movement should change animation frame
  frame = (frame + 1) % 2;
}

void Invader::render()
{
  SDL_Rect clip = {12 * frame, 16, 12, 8};
  if (type == 2) clip.y = 32;
  else if (type == 1) clip.y = 24;

  app->renderClip(x, y, clip);
}
