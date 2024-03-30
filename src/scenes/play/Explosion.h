#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "utils/Timer.h"


struct Explosion
{
  int x, y;
  SDL_Rect clip;
  Timer clock;

  Explosion(int x, int y, Uint32 lifespan, const SDL_Rect& clip);

  Explosion &operator=(const Explosion &other)
  {
    x = other.x;
    y = other.y;
    clip = other.clip;
    clock = other.clock;

    return *this;
  }

  bool hasFinished() const;

  void update();
  void render() const;
};

#endif // EXPLOSION_H
