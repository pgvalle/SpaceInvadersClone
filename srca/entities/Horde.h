#pragma once

#include <vector>

#include "utils/Timer.h"
#include "Invader.h"

struct Horde {
private:
  Timer frozen_timer;
  int xrel, xrel_count;

  void update_starting();

  void update_moving();

public:
  enum State {
    STARTING,
    MOVING
  } state;
  std::vector<Invader> invaders;

  Horde();

  void freeze_temporarily(); // when an invader is shot
  
  bool is_finished();

  void update();
  void render();
};
