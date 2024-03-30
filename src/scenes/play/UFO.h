#pragma once

#include "utils/Timer.h"

struct UFO {
private:
  Timer clock; // for spawn, motion and death

public:
  enum State {
    VISIBLE,
    HIDDEN,
    DYING, // showing death animation
    DEAD, // show its score value
    AFTERLIFE // they are in a better place
  } state;
  int x, xVel;
  int scoreValue;

  UFO();

  // external influences

  void process_strike();

  void update();
  void render();
};
