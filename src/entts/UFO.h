#pragma once

#include "common.h"
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
  int score;

  UFO();

  Explosion *onHit();
  SDL_Rect getHitbox() const;
  void onUpdate(float dt);
  void onRender() const;
};
