#pragma once

#include "utils/Timer.h"


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

  bool isDead();

  bool checkAndProcessHit(const SDL_Rect &hitbox);

  // Shot shoot();

  void update();
  void render();
};
