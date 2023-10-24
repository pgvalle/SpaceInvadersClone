#pragma once

#include "utils.h"
#include "fsms.h"

#include "Invader.h"

class Horde {
private:
  std::vector<Invader> m_invaders;
  HordeState m_state;

  Timer m_motion_timer;
  int m_motion_x, m_motion_x_count;

public:
  Horde() {
    m_state = HordeState::STARTING;
    m_motion_timer = Timer(55 * 16);
  }

  void force_wait() {
    // timeout will decrease over invader amount decrease
    const Uint64 new_timeout = m_invaders.size() * 16;
    m_motion_timer.reset(new_timeout);
  }

  void update(Uint64 delta) {
    switch (m_state) {
    case HordeState::STARTING:
      if (m_invaders.size() < 55) {
        const int row = 4 - m_invaders.size() / 11;
        const int col = m_invaders.size() % 11;
        m_invaders.push_back(Invader(row, col));
      } else {
        // done. Now move right
        m_state = HordeState::MOVING;
        m_motion_x = 2;
        m_motion_x_count = 8;
      }
      break;
    case HordeState::MOVING:
      m_motion_timer.update(delta);
      if (m_motion_timer.has_timed_out()) {
        // before swaping directons should move down
        int motion_x = m_motion_x, motion_y = 0;
        if (++m_motion_x_count == 16) {
          motion_x = 0;
          motion_y = 2;
          m_motion_x = -m_motion_x;
          m_motion_x_count = 0;
        }
        // move in fact
        for (Invader& invader : m_invaders) {
          invader.move(motion_x, motion_y);
        }
        
        m_motion_timer.reset();
      }
      break;
    }
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    for (Invader& invader : m_invaders) {
      invader.render(renderer, atlas);
    }
  }
};
