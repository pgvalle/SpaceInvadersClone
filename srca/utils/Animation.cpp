#include "Animation.h"
#include "App.h"

Animation::Animation(const SDL_Rect& clip_, Uint32 frame_duration, int nof_frames_) {
  clip = clip_;
  current_frame = 0;
  nof_frames = nof_frames_;
  frame_clock = Timer(frame_duration);
}


void Animation::update() {
  frame_clock.update();
  if (frame_clock.has_timed_out()) {
    current_frame = (current_frame + 1) % nof_frames;
    frame_clock.reset();
  }
}

void Animation::render(int x, int y) const {
  // create new clip that is going to be displayed
  SDL_Rect temp = clip;
  temp.x += clip.w * current_frame;
  App::render_clip(x, y, temp);
}
