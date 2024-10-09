#pragma once

#include "util/Timer.h"
#include "Shot.h"

enum CannonState
{
  ALIVE,
  DYING, // animation appears here
  DEAD
};

class Cannon
{
private:
  enum CannonState state;
  int x;
  int deathFrame;
  Timer clock1, clock2;

public:
  Cannon();

  Shot shoot() const;

  bool isDead() const
  {
    return state == DEAD;
  }

  bool checkAndProcessHit(const SDL_Rect &hitbox);

  void onUpdate(float dt);
  void onRender() const;
};
