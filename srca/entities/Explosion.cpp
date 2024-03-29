#include "Explosion.h"
#include "App.h"

Explosion::Explosion(int $x, int $y, Uint32 $lifespan, const SDL_Rect& $clip) {
  x = $x;
  y = $y;
  clip = $clip;
  lifespan = Timer($lifespan);
}

bool Explosion::has_finished() {
  return lifespan.has_timed_out();
}

void Explosion::update() {
  lifespan.update();
}

void Explosion::render() {
  App::render_clip(x, y, clip);
}