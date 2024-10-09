#include "Explosion.h"
#include "SIC.h"

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
