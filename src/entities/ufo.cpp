#include "ufo.h"
#include "explosion.h"
#include "globals.h"

#define UFO_Y (5 * TILE)
#define UFO_LLIMIT (2 * TILE)
#define UFO_RLIMIT (16 * TILE)

#define UFO_TIME_EXPLODING 0.5
#define UFO_TIME_TO_RESPAWN Timer::getRandomTimeout(10, 15)

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
  
  Explosion *e = new Explosion(x, UFO_Y, UFO_TIME_EXPLODING);
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
        vx = 0.6;
      }
      else
      {
        x = UFO_RLIMIT;
        vx = -0.6;
      }      
    }
    break;

  case ALIVE:
  {
    x += vx * delta;
    const int xi = roundf(x);
  
    if (xi < UFO_LLIMIT || xi > UFO_RLIMIT) // out of bounds
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

  SDL_FRect fr = {x + 4, UFO_Y, 2 * TILE, TILE};
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
