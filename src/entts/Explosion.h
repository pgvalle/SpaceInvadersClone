#ifndef ENTITIES_EXPLOSION
#define ENTITIES_EXPLOSION

#include "common.h"

struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Timer clock;
};

#endif // ENTITIES_EXPLOSION
