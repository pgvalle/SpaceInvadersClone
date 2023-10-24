#pragma once

#include "utils.h"

class Tourist {
private:
  int m_x;
  Timer m_motion_timer, m_spawn_timer;

  int m_score_earned;

public:
  Tourist() {
    m_x = -10000;
    m_motion_timer = Timer(60);
    m_spawn_timer = Timer();
  }

  void despawn() {
    m_x = -10000;
    const Uint64 new_timeout = 1008 * (rand() % (30 - 20 + 1) + 20);
    m_spawn_timer.reset(new_timeout);
  }

  void update() {

  }

  void render() {

  }
};