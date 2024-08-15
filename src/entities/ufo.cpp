#include "ufo.h"
#include "explosion.h"
#include "globals.h"

#define UFO_Y (5 * TILE)
#define UFO_LLIMIT (2 * TILE)
#define UFO_RLIMIT (26 * TILE)

#define UFO_SPEED 40

#define UFO_TIME_EXPLODING 0.5
#define UFO_TIME_TO_RESPAWN Timer::getRandomTimeout(0, 3)

UFO::UFO()
{
  state = AWAY;
  clock.reset(UFO_TIME_TO_RESPAWN); // spawn after 10s
}

Explosion *UFO::onHit(const SDL_Rect &rect)
{
  if (state != ALIVE)
    return nullptr;

  const SDL_Rect ufoRect = {(int)roundf(x), UFO_Y, 2 * TILE, TILE};

  if (!SDL_HasIntersection(&rect, &ufoRect))
    return nullptr;

  state = EXPLODING;
  clock.reset(UFO_TIME_EXPLODING);

  // offset bc explosion sprite width is 8 pixels bigger
  Explosion *e = new Explosion(x - 4, UFO_Y, UFO_TIME_EXPLODING);
  e->clip = {16, 0, 24, 8};

  return e;
}

void UFO::update()
{
  const float delta = 1.0 / FPS;

  switch (state)
  {
  case AWAY:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = ALIVE;
      scoreValue = 100; // TODO: randomize score value

      // randomly choose a corner
      if (rand() % 2)
      {
        x = UFO_LLIMIT;
        vx = UFO_SPEED;
      }
      else
      {
        x = UFO_RLIMIT;
        vx = -UFO_SPEED;
      }
    }
    break;
  case ALIVE:
  {
    x += vx * delta;

    const int ix = roundf(x);
    if (ix < UFO_LLIMIT || ix > UFO_RLIMIT) // out of bounds
    {
      state = AWAY;
      clock.reset(UFO_TIME_TO_RESPAWN);
    }
    break;
  }
  case EXPLODING:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = SHOWING_SCORE;
      clock.reset(2);
      addToScore(scoreValue);
    }
    break;
  case SHOWING_SCORE:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = AWAY;
      clock.reset(UFO_TIME_TO_RESPAWN);
    }
    break;

  default:
    break;
  }
}

void UFO::render() const
{
  SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);

  switch (state)
  {
  case ALIVE:
  {
    const SDL_Rect src = {0, 0, 16, 8},
                   dst = {(int)roundf(x), UFO_Y, 2 * TILE, TILE};
    SDL_RenderCopy(ren, atlas, &src, &dst);
    break;
  }
  case EXPLODING:
  {
    const SDL_Rect src = {0, 0, 24, 8},
                   dst = {(int)roundf(x) - 4, UFO_Y, 3 * TILE, TILE};
    SDL_RenderCopy(ren, atlas, &src, &dst);
    break;
  }
  case SHOWING_SCORE:
    // NAGE::renderText(xi, Y, "%3d", scoreValue);
    break;
  default:
    break;
  }
}
