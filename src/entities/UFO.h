#pragma once

#include "utils/Timer.h"


struct UFO
{
private:
  enum State
  {
    ALIVE,
    DYING1, // showing death animation
    DYING2, // show its score value
    DEAD    // they are in a better place
  } state;
  int x, xVel;
  int scoreValue;
  Timer clock; // for spawn, motion and death

public:
  UFO();

  bool isDead() const
  {
    return state == DEAD;
  }

  bool checkAndProcessHit(const SDL_Rect &hitbox);

  void update();
  void render() const;
};
