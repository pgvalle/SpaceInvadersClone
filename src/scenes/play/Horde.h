#pragma once

#include <vector>

#include "utils/Timer.h"
#include "Explosion.h"
#include "Shot.h"


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

  int xVel;
  Timer delayer;

  std::vector<Invader> invaders;

public:
  Horde();

  Shot shoot();

  bool isDestroyed();

  Explosion checkAndProcessInvaderHit(const SDL_Rect& hitbox);


  void update();
  void render();
};
