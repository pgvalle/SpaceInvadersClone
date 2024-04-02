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

    int getScoreValue() const
    {
      return 10 * (3 - type);
    }

    SDL_Rect getHitbox() const;

    void move(int xOff, int yOff);
    void render() const;
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

  Shot shoot() const;

  bool isDestroyed() const
  {
    return invaders.empty();
  }

  Explosion checkAndProcessHit(const SDL_Rect& hitbox);


  void update();
  void render() const;
};
