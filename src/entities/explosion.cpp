#include "explosion.h"

Explosion::Explosion(int $x, int $y, float lifespan, const SDL_Rect& $clip)
{
  x = $x;
  y = $y;
  clip = $clip;
  clock.reset(lifespan);
}
