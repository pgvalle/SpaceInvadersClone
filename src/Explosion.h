#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "utils.h"

struct Explosion {
  int x, y;
  SDL_Rect clip;
  Timer lifespan;

  Explosion(int x_, int y_, Uint64 lifespan_, const SDL_Rect& clip_) {
    x = x_;
    y = y_;
    clip = clip_;
    lifespan = Timer(lifespan_);
  }

  bool has_finished() {
    return lifespan.has_timed_out();
  }

  void update(Uint64 delta) {
    lifespan.update(delta);
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    render_clip(x, y, clip, renderer, atlas);
  }
};

#endif // EXPLOSION_H