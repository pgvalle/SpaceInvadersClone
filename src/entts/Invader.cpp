#include "Invader.h"
#include "glob.h"

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

Explosion *Invader::onHit(const SDL_Rect &rect) const
{
  SDL_Rect iRect = getRect();
  if (!SDL_HasIntersection(&rect, &iRect))
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

SDL_Rect Invader::getRect() const
{
  SDL_Rect iRect = {x, y, 8, 8};
  if (type == 2)
    iRect = {x - 2, y, 12, 8};
  else if (type == 1)
    iRect = {x - 1, y, 11, 8};
  
  return iRect;
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
  SDL_Rect src = getRect();
  const SDL_Rect dst = {animationFrame * dst.w, 16 + 8 * type, dst.w, dst.h};

  SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
}