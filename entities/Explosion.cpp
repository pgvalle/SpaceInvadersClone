#include "Explosion.h"
#include "app/App.h"


Explosion::Explosion(int x_, int y_, Uint32 lifespan, const SDL_Rect &clip_)
{
  x = x_;
  y = y_;
  clip = clip_;
  clock.reset(lifespan);
}

bool Explosion::hasFinished() const
{
  return clock.hasTimedOut();
}

void Explosion::update()
{
  clock.update();
}

void Explosion::render() const
{
  renderClip(x, y, clip);
}
