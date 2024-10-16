#pragma once

#include "common.h"
#include "Invader.h"
#include "Explosion.h"

struct Horde
{
  std::vector<Invader> invaders;
  int x, vx;
  bool populating;
  Timer clock;

  Horde();

  void onUpdate(float dt);
  void onRender() const;
};
