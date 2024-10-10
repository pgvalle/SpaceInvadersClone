#include "Explosion.h"
#include "SIC.h"

void Explosion::draw() const
{
  const SDL_Rect dst = {x, y, clip.w, clip.h};
  SDL_RenderCopy(sic->renderer, sic->atlas, &clip, &dst);
}
