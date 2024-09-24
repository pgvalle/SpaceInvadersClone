#ifndef ENTITIES_UFO
#define ENTITIES_UFO

#include "util/Timer.h"
#include "Explosion.h"

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
  float x, vx;
  Timer clock;

  UFO();

  Explosion *onHit(const SDL_Rect &rect);
  void update(float dt);
  void render() const;
};

#endif  // ENTITIES_UFO
