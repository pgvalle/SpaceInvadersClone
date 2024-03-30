#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "utils/Timer.h"


struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Timer clock;

  Explosion(int x, int y, Uint32 lifespan, const SDL_Rect& clip);

  bool hasFinished();

  void update();
  void render();
};

#endif // EXPLOSION_H
