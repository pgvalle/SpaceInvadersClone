#pragma once

#include <vector>

#include "utils/Timer.h"
#include "Explosion.h"


struct Horde {
private:
  struct Invader
  {
    int type, frame;
    int x, y;

    Invader(int col, int row);

    SDL_Rect getHitbox();

    void move(int xOff, int yOff);
    void render();
  };

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

  Explosion checkAndProcessInvaderHit(const SDL_Rect& hitbox);


  void update();
  void render();
};
