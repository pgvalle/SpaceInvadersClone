#pragma once

#include "utils.h"
#include "fsms.h"

#include "Invader.h"

struct Horde {
private:
  Timer motion_timer;
  int xrel, xrel_count;

  void update_starting(Uint64 delta);

  void update_moving(Uint64 delta);

public:
  enum State {
    STARTING,
    MOVING
  } state;
  std::vector<Invader> invaders;

  Horde();

  void freeze_temporarily();

  void update(Uint64 delta);
  void render(SDL_Renderer* renderer, SDL_Texture* atlas);
};
