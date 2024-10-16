#include "Shot.h"
#include "SIC.h"

Explosion *Shot::collisionCallback(const SDL_Rect &expClip)
{
  Explosion *exp = new Explosion;
  exp->x = x;
  exp->y = y;
  exp->clip = expClip;
  exp->lifespan.reset(0.5);
  return exp;
}

SDL_Rect Shot::getCollider() const
{
  return {(int)round(x), (int)round(y), 1, 5};
}

void Shot::draw() const
{
  const SDL_Rect collider = getCollider();
  SDL_SetRenderDrawColor(sic->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(sic->renderer, &collider);
}
