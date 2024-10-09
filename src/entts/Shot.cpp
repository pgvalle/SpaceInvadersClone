#include "Shot.h"
#include "SIC.h"

Explosion *Shot::onHit()
{
  Explosion *e = new Explosion;
  e->x = x - 3;
  e->y = y;
  e->clip = expClip;
  e->lifespan.reset(0.5);
}

SDL_Rect Shot::getHitbox() const
{
  return {x, y, 1, 5};
}

void Shot::onUpdate(float dt)
{
  y += dt * vy;
}

void Shot::onRender() const
{
  const SDL_Rect dst = {x, y, clip.w, clip.h};
  SDL_RenderCopy(g->ren, g->atlas, &clip, &dst);
}
