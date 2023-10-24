#pragma once

#include "utils.h"

class InvaderShot {
private:
  int m_x, m_y;
  SDL_Rect m_clip;

  Timer m_motion_timer;
  Timer m_animation_timer;

public:
  InvaderShot(int x, int y, const SDL_Rect& clip) {
    m_x = x;
    m_y = y;
    m_clip = clip;
    m_motion_timer = Timer(0);
    m_animation_timer = Timer(96);
  }

  void update(Uint64 delta) {
    // animate it
    m_animation_timer.update(delta);
    if (m_animation_timer.has_timed_out()) {
      m_clip.x = 24 + (m_clip.x - 21) % 12;
      m_animation_timer.reset();
    }
    // move it
    m_motion_timer.update(delta);
    if (m_motion_timer.has_timed_out()) {
      m_y += 2;
      m_motion_timer.reset();
    }
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    render_clip(m_x, m_y, m_clip, renderer, atlas);
  }
};
