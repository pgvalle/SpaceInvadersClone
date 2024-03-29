#pragma once

#include "utils/Timer.h"

#include <SDL.h>

// always on loop
class Animation {
private:
  SDL_Rect clip;
  int current_frame, nof_frames;
  Timer frame_clock;

public:
  Animation() {} // default constructor
  Animation(const SDL_Rect& clip, Uint32 frame_duration, int nof_frames);

  void update();
  void render(int x, int y) const;
};
