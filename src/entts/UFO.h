#pragma once

#include "common.h"
#include "util/Ticker.h"
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
  float x, dx;
  int score;
  Ticker ticker;

  UFO();

  Explosion *collisionCallback();
  SDL_Rect getCollider() const;
  void tick();
  void draw() const;
};
