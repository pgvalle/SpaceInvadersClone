#ifndef ENTITIES_UFO
#define ENTITIES_UFO

#include <common.h>
#include "explosion.h"

enum UFOState
{
  UFO_AWAY,
  UFO_ALIVE,
  UFO_EXPLODING,
  UFO_SHOWING_SCORE,
  UFO_DEAD
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

extern UFO ufo;

#endif  // ENTITIES_UFO
