#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "utils.h"

class Explosion {
private:
  int m_x, m_y;
  SDL_Rect m_clip;

  Timer m_lifespan;

public:
  Explosion(int x, int y, Uint64 lifespan, const SDL_Rect& clip) {
    m_x = x;
    m_y = y;
    m_clip = clip;
    m_lifespan = Timer(lifespan);
  }

  bool has_finished() {
    return m_lifespan.has_timed_out();
  }

  void update(Uint64 delta) {
    m_lifespan.update(delta);
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    render_clip(m_x, m_y, m_clip, renderer, atlas);
  }
};

#endif // EXPLOSION_H