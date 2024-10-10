#pragma once

#include "common.h"

struct Scene
{
  virtual ~Scene() {};

  virtual void processEvent(const SDL_Event &event) = 0;
  virtual void tick(float dt) = 0;
  virtual void draw() const = 0;
};
