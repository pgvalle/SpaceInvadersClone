#include "Explosion.h"
#include "SIC.h"

Explosion *Explosion::onHit(const SDL_Rect &rect)
{
  return NULL;
}

SDL_Rect Explosion::getHitbox() const
{
  return {0, 0, 0, 0};
}

void Explosion::onTick(float dt)
{
}

void Explosion::onUpdate(float dt)
{
  lifespan.update(dt);
}

void Explosion::onRender() const
{
  const SDL_Rect dst = {x, y, clip.w, clip.h};
  SDL_RenderCopy(g->ren, g->atlas, &clip, &dst);
}
