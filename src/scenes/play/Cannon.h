#pragma once

#include "utils/Timer.h"
#include "Shot.h"


class Cannon
{
private:
  enum State
  {
    ALIVE,
    DYING, // animation appears here
    DEAD
  } state;
  int x;
  int deathFrame;
  Timer clock1, clock2;

public:
  Cannon();

  Shot shoot();

  bool isDead();

  bool checkAndProcessHit(const SDL_Rect &hitbox);

  void update();
  void render();
};
