#pragma once

#include "utils.h"
#include "fsms.h"

#include "Invader.h"

struct Horde {
  HordeState state;
  std::vector<Invader> invaders;

  Timer motion_timer;
  int x_motion, x_motion_count;

  Horde() {
    state = HordeState::STARTING;
    motion_timer = Timer(55 * 16);
  }

  void force_wait() {
    // timeout will decrease with invader amount decreasing
    const Uint64 new_timeout = invaders.size() * 16;
    motion_timer.reset(new_timeout);
  }

  void update(Uint64 delta) {
    switch (state) {
    case HordeState::STARTING:
      if (invaders.size() < 55) {
        invaders.push_back(Invader(
          4 - invaders.size() / 11, invaders.size() % 11
        ));
      } else {
        // done. Now move right
        state = HordeState::MOVING;
        x_motion = 2;
        x_motion_count = 8;
      }
      break;
    case HordeState::MOVING:
      motion_timer.update(delta);
      if (motion_timer.has_timed_out()) {
        // before swaping directons should move down
        int x_motion = x_motion, motion_y = 0;
        if (++x_motion_count == 16) {
          x_motion = 0;
          motion_y = 2;
          x_motion = -x_motion;
          x_motion_count = 0;
        }
        // move in fact
        for (Invader& invader : invaders) {
          invader.move(x_motion, motion_y);
        }
        
        motion_timer.reset();
      }
      break;
    }
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    for (Invader& invader : invaders) {
      invader.render(renderer, atlas);
    }
  }
};
