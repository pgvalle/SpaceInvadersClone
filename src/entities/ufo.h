#ifndef ENTITIES_UFO
#define ENTITIES_UFO

#include <common.h>
#include "explosion.h"

enum UFOState
{
  AWAY,
  ALIVE,
  EXPLODING,
  SHOWING_SCORE,
  DEAD
};

struct UFO
{
  UFOState state;
  Timer clock;
  float x, vx;
  int scoreValue;

  UFO();

  Explosion *onHit(const SDL_Rect &rect);
  void update();
  void render() const;
};

DECL(UFO, ufo);

#endif  // ENTITIES_UFO
