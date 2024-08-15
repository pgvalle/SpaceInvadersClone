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

void Invader::render() const
{
  SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
  const SDL_Rect rect = {x, y, 8, 8};
  SDL_RenderFillRect(ren, &rect);
}

Horde::Horde()
{
  clock.reset(0.033);
  populating = true;
}

Explosion *Horde::onHit(const SDL_Rect &rect)
{
  for (int i = 0; i < invaders.size(); i++)
  {
    const SDL_Rect invaderRect = invaders[i].getRect();
    if (!SDL_HasIntersection(&rect, &invaderRect))
      continue;

    Explosion *e = new Explosion(invaders[i].x, invaders[i].y, 0.2);
    e->clip = {28, 16, 13, 8};

    addToScore(invaders[i].getScoreValue());

    invaders.erase(invaders.begin() + i);

    return e;
  }

  return nullptr;
}

void Horde::update()
{
  clock.update();

  if (!clock.hasTimedOut())
    return;

  if (populating && invaders.size() < 55) // still populating
  {
    clock.reset();
    
    const int count = invaders.size();
    // start from the bottom-left. Iterate line first
    const int row = 10 - count / 11;
    const int col = count % 11;
    invaders.push_back(Invader(row, col));
    return;
  }

  if (populating) // now populated
  {
    clock.reset(55 * 0.033);
    populating = false;
    x = 7;
    vx = 2; // start going right
    return;
  }

  // time to move horde

  int vy = 0;
  if (++x == 16)
  {
    x = 0;
    vx = -vx;
    vy = 8;
  }

  for (Invader &i : invaders)
    i.move(vx, vy);
  // move and check movements for an inversion
  clock.reset();    
}

void Horde::render() const
{
  for (const Invader &i : invaders)
    i.render();
}

Horde horde;
