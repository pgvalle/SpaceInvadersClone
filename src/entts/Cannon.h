#pragma once

#include "common.h"
#include "util/Timer.h"
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
  float x;
  int deathFrame;
  Timer clock1, clock2;

  Cannon();

  void onHit();
  SDL_Rect getHitbox() const;
  void onUpdate(float dt);
  void onRender() const;
  Shot *shoot() const;
};
