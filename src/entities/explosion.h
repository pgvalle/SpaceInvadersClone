#ifndef ENTITIES_EXPLOSION
#define ENTITIES_EXPLOSION

#include <common.h>

struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Timer clock;

  Explosion(int x, int y, float lifespan, const SDL_Rect &clip);
};

DECL(std::vector<Explosion>, esplosions);

#endif // ENTITIES_EXPLOSION
