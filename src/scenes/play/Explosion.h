#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "utils/Timer.h"


class Explosion
{
private:
  int x, y;
  SDL_Rect clip;
  Timer clock;

public:
  Explosion(int x, int y, Uint32 lifespan, const SDL_Rect& clip);

  bool hasFinished() const;

  void update();
  void render() const;
};

#endif // EXPLOSION_H
