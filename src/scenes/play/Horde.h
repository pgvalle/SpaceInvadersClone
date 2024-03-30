#pragma once

#include <vector>

#include "utils/Timer.h"
#include "Invader.h"

struct Horde {
private:
  Timer delayer;
  int xVel, xStepCount;

public:
  enum State {
    STARTING,
    MOVING
  } state;

  std::vector<Invader> invaders;

  Horde();

  bool isDestroyed();

  // external influences

  void forceDelayOnHit(); // when an invader is shot


  void update();
  void render();
};
