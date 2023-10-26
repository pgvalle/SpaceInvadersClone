#pragma once

#include "utils.h"
#include "fsms.h"

struct Tourist {
  TouristState state;
  int x, xvel;
  Timer timer; // for spawn, motion and death

  int score_value;

  Tourist() {
    state = TouristState::HIDDEN;
    x = -10000;
    xvel = 0;
    timer = Timer(15000); // spawn
    score_value = 100;
  }

  void update(Uint64 delta) {
    switch (state) {
    case TouristState::HIDDEN:
      timer.update(delta);
      if (timer.has_timed_out()) {
        state = TouristState::VISIBLE;
        x = 0;
        xvel = 1;
        // will be used as motion timer
        timer.reset(60);
      }
      break;
    case TouristState::VISIBLE:
      timer.update(delta);
      if (timer.has_timed_out()) {
        x += xvel;
      }
      break;
    case TouristState::DYING:
      
      break;
    }
  }

  void render() {

  }
};