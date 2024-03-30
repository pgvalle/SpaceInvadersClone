#pragma once

#include "utils/Timer.h"

class Cannon {
private:
public:
  enum State {
    ALIVE,
    DYING, // animation appears here
    DEAD
  } state;
  int x;
  int deathFrame;
  Timer clock1, clock2;

  Cannon();

  bool isDead();

  void checkAndProcessHit(const SDL_Rect &hitbox);

  // Shot shoot();

  void update();
  void render();
};
