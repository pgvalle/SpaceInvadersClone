#pragma once

#include <vector>

#include "utils/Timer.h"
#include "Invader.h"

struct Horde {
private:
  Timer frozen_timer;
  int xrel, xrelCount;

  void updateStarting();

  void updateMoving();

public:
  enum State {
    STARTING,
    MOVING
  } state;

  std::vector<Invader> invaders;

  Horde();

  void freezeTemporarily(); // when an invader is shot
  
  bool isFinished();

  void update();
  void render();
};
