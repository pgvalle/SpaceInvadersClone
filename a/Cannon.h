#pragma once

#include "common.h"
#include "util/Ticker.h"
#include "Shot.h"

enum CannonState
{
  ALIVE,
  DYING, // animation appears here
  DEAD
};

struct Cannon
{
  enum CannonState state;
  float x, dx;
  int lives;
  int deathFrame;
  Ticker clock1, clock2;

  Cannon();

  // void onHit();
  // SDL_Rect getHitbox() const;
  void tick();
  void draw() const;
  //Shot *shoot() const;
};
