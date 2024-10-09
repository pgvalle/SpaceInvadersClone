#include "Shot.h"
#include "SIC.h"

void Shot::onUpdate(float dt)
{
  y += dt * vy;
}

void Shot::onRender() const
{
  const SDL_Rect dst = {x, y, clip.w, clip.h};
  SDL_RenderCopy(g->ren, g->atlas, &clip, &dst);
}
