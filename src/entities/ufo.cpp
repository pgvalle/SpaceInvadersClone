#include "ufo.h"
#include "globals.h"

#define Y (5 * TILE)

UFO::UFO()
{
  state = AWAY;
  clock.reset(10); // spawn after 10s
}

Explosion *UFO::onHit(const SDL_Rect &rect)
{
  // not even there to be hit
  if (state != ALIVE)
    return nullptr;

  const SDL_Rect ufoRect = {roundf(x) + 4, Y, 2 * TILE, TILE};
  if (SDL_HasIntersection(&rect, &ufoRect))
  {
    state = EXPLODING;
    clock.reset(0.3);
    
    return new Explosion(x, Y, 2, {0, 0, rect.w, rect.h});
  }

  return nullptr;
}

void UFO::update()
{
  const float delta = 1.0 / FPS;

  switch (state)
  {
  case AWAY:
    clock.update(delta);
    if (clock.hasTimedOut())
    {
      state = ALIVE;
      // randomly choose a corner
      left = rand() % 2;
      x = left ? (24 * 8) : 16;
      scoreValue = 100; // TODO: randomize score value
    }
    break;

  case ALIVE:
  {
    const float vx = 40.0f * (left ? -1 : 1); // 1 canvas_unit/s
    x += vx * delta;
    const int xi = roundf(x);
    if (xi < 16 || xi > 24 * 8) // out of bounds
    {
      state = AWAY;
      clock.reset(10);
    }
    break;
  }

  case EXPLODING:
    // animation frame should change
    clock.update(delta);
    if (clock.hasTimedOut())
    {
      state = SHOWING_SCORE;
      clock.reset(2);
      addToScore(scoreValue);
    }
    break;

  case SHOWING_SCORE:
    clock.update(delta);
    if (clock.hasTimedOut())
    {
      state = AWAY;
      clock.reset(10);
    }
    break;

  default:
    break;
  }
}

void UFO::render() const
{
  SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

  SDL_FRect fr = {x + 4, Y, 2 * TILE, TILE};
  switch (state)
  {
  case ALIVE:
    SDL_RenderFillRectF(ren, &fr);
    break;
  case EXPLODING:
    SDL_RenderFillRectF(ren, &fr);
    break;
  case SHOWING_SCORE:
    // NAGE::renderText(xi, Y, "%3d", scoreValue);
    break;
  default:
    break;
  }
}
