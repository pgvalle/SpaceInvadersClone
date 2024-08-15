#include "explosions.h"
#include "globals.h"

Explosion::Explosion(int $x, int $y, float lifespan)
{
  x = $x;
  y = $y;
  clock.reset(lifespan);
}

void updateExplosions()
{
  for (int i = 0; i < explosions.size(); i++)
  {
    Explosion *e = explosions[i];

    e->clock.update();
    if (!e->clock.hasTimedOut())
      continue;
    
    explosions.erase(explosions.begin() + i--);
    delete e;
  }
}

void renderExplosions()
{
  for (Explosion *e : explosions)
  {
    const SDL_Rect rect = {e->x, e->y, e->clip.w, e->clip.h};
    SDL_RenderCopy(ren, atlas, &rect, &e->clip);
  }
}
