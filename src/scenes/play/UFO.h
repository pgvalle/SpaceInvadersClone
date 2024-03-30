#pragma once

#include "utils/Timer.h"

struct UFO {
private:
  Timer clock; // for spawn, motion and death

public:
  enum State {
    ALIVE,
    DYING1, // showing death animation
    DYING2, // show its score value
    DEAD // they are in a better place
  } state;
  int x, xVel;
  int scoreValue;

  UFO();

  // external influences

  bool checkAndProcessHit(const SDL_Rect &hitbox);

  void update();
  void render();
};
