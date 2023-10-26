#pragma once

#include "utils.h"

struct InvaderShot {
  int x, y;
  SDL_Rect clip;

  Timer motion_timer;
  Timer animation_timer;

public:
  InvaderShot(int x_, int y_, const SDL_Rect& clip_) {
    x = x_;
    y = y_;
    clip = clip_;
    motion_timer = Timer(0);
    animation_timer = Timer(96);
  }

  void update(Uint64 delta) {
    // animate it
    animation_timer.update(delta);
    if (animation_timer.has_timed_out()) {
      clip.x = 24 + (clip.x - 21) % 12;
      animation_timer.reset();
    }
    // move it
    motion_timer.update(delta);
    if (motion_timer.has_timed_out()) {
      y += 2;
      motion_timer.reset();
    }
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    render_clip(x, y, clip, renderer, atlas);
  }
};
