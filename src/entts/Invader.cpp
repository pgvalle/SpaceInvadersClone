#include "Invader.h"
#include "Explosion.h"
#include "SIC.h"

Invader::Invader(int row, int col)
{
  type = 0;
  if (row > 2)
    type = 2;
  else if (row > 1)
    type = 1;

  x = 2 + (3 + 2 * col) * TILE;
  y = 0 + (8 + 2 * row) * TILE;
  clipIndex = 0;
}

Explosion *Invader::onHit(const SDL_Rect &rect)
{
  SDL_Rect hb = getHitbox();
  if (!SDL_HasIntersection(&rect, &hb))
    return NULL;

  const int scoreValue =  10 * (3 - type);
  g->add2Score(scoreValue);

  Explosion *e = new Explosion;
  e->x = x;
  e->y = y;
  e->clip = {28, 16, 13, 8};
  e->lifespan.reset(0.5);
  return e;
}

SDL_Rect Invader::getHitbox() const
{
  if (type == 2)
    return {x - 2, y, 12, 8};
  else if (type == 1)
    return {x - 1, y, 11, 8};
  return {x, y, 8, 8};
}

void Invader::onTick(float dt)
{
}

void Invader::onUpdate(float dt)
{
}

void Invader::onRender() const
{
  const SDL_Rect dst = getHitbox();
  const SDL_Rect src = {clipIndex * dst.w, 16 + 8 * type, dst.w, dst.h};
  SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
}

void Invader::move(int xOff, int yOff)
{
  x += xOff;
  y += yOff;
  // each movement should change animation frame
  clipIndex = (clipIndex + 1) % 2;
}
