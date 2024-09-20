#ifndef ENTITIES_HORDE
#define ENTITIES_HORDE

#include <common.h>
#include "invader.h"
#include "explosion.h"

struct Horde
{
  Timer clock;
  int x, vx;
  bool populating;
  std::vector<Invader> invaders;

  Horde();

  Explosion *onHit(const SDL_Rect &rect);
  void update();
  void render() const;
};

extern Horde horde;

#endif  // ENTITIES_HORDE
