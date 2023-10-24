#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <vector>

class Timer {
private:
  Uint64 m_time_passed;
  Uint64 m_timeout;

public:
  Timer() {} // default constructor
  Timer(Uint64 timeout) {
    m_time_passed = 0;
    m_timeout = timeout;
  }

  void update(Uint64 delta) {
    if (m_timeout < m_time_passed) {
      m_time_passed += delta;
    }
  }

  void reset(Uint64 new_timeout = 0) {
    m_time_passed = 0;
    m_timeout = (new_timeout == 0 ? m_timeout : new_timeout);
  }

  bool has_timed_out() const {
    return m_timeout < m_time_passed;
  }
};

bool is_key_pressed(int key) {
  const Uint8* keys = SDL_GetKeyboardState(NULL);
  return keys[key];
}

void render_clip(int x, int y, const SDL_Rect& clip, SDL_Renderer* renderer,
  SDL_Texture* atlas)
{
  const SDL_Rect scale = { x, y, clip.w, clip.h };
  SDL_RenderCopy(renderer, atlas, &clip, &scale);
}

#endif // UTILS_H