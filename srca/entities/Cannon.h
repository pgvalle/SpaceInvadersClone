#pragma once

#include "utils/Timer.h"
#include "utils/Animation.h"

class Cannon {
private:
  Animation death_anim;

public:
  enum State {
    STARTING,
    ALIVE,
    DYING, // animation appears here
    DEAD
  } state;
  int x;
  int lives;
  Timer timer;

  Cannon();

  void update();
  void render();
};