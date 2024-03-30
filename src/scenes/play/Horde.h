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

  enum State {
    STARTING,
    MOVING
  } state;

  Timer delayer;
  int xVel, xStepCount;

  std::vector<Invader> invaders;

public:
  Horde();

  bool isDestroyed();

  // external influences

  Explosion checkAndProcessInvaderHit(const SDL_Rect& hitbox);


  void update();
  void render();
};
