#ifndef ENTITIES_EXPLOSION
#define ENTITIES_EXPLOSION

#include "common.h"
#include "util/Timer.h"

struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Timer lifespan;
};

#endif // ENTITIES_EXPLOSION
