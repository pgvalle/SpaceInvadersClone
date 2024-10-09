#ifndef ENTITIES_UFO
#define ENTITIES_UFO

#include "util/Timer.h"
#include "Entity.h"

enum UFOState
{
  UFO_AWAY,
  UFO_ALIVE,
  UFO_EXPLODING,
  UFO_SHOWING_SCORE,
  UFO_DEAD
};

struct UFO : Entity
{
  UFOState state;
  float x, vx;
  Timer clock;
  int score;

  UFO();

  Explosion *onHit(const SDL_Rect &rect) override;
  SDL_Rect getHitbox() const override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;
};

#endif  // ENTITIES_UFO
