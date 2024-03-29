#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "utils/Timer.h"

class Explosion {
private:
  int x, y;
  SDL_Rect clip;
  Timer lifespan;

public:
  Explosion(int x, int y, Uint32 lifespan, const SDL_Rect& clip);

  bool has_finished();

  void update();
  void render();
};

#endif // EXPLOSION_H