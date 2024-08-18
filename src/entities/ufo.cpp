#include "ufo.h"
#include "globals.h"

#define UFO_Y (5 * TILE)
#define UFO_LLIMIT (TILE)
#define UFO_RLIMIT (24 * TILE)

#define UFO_SPEED 40

#define UFO_TIME_EXPLODING 0.5
#define UFO_TIME_TO_RESPAWN Timer::getRandomTimeout(10, 15)

UFO::UFO()
{
  state = UFO_AWAY;
  clock.reset(UFO_TIME_TO_RESPAWN); // spawn after 10s
}

Explosion *UFO::onHit(const SDL_Rect &rect)
{
  if (state != UFO_ALIVE)
    return nullptr;

  const SDL_Rect ufoRect = {(int)roundf(x), UFO_Y, 2 * TILE, TILE};

  if (!SDL_HasIntersection(&rect, &ufoRect))
    return nullptr;

  state = UFO_EXPLODING;
  clock.reset(UFO_TIME_EXPLODING);

  // offset bc explosion sprite width is 8 pixels bigger
  Explosion *e = new Explosion(x - 4, UFO_Y, UFO_TIME_EXPLODING);
  e->clip = {16, 0, 24, 8};

  return e;
}

void UFO::update()
{
  const float delta = 1.0 / FPS;

  clock.update();

  switch (state)
  {
  case UFO_AWAY:
    if (!clock.hasTimedOut())
      break;

    state = UFO_ALIVE;
    clock.reset(1e10); // so that we don't get stuck in alive state
    scoreValue = 100;  // TODO: randomize score value

    // randomly choose a corner to spawn in: left or right
    if (rand() % 2) // left
    {
      x = UFO_LLIMIT;
      vx = UFO_SPEED;
    }
    else
    {
      x = UFO_RLIMIT;
      vx = -UFO_SPEED;
    }
    break;
  
  case UFO_ALIVE:
    x += vx * delta;
    if (round(x) < UFO_LLIMIT || round(x) > UFO_RLIMIT) // out of bounds
    {
      state = UFO_AWAY;
      clock.reset(UFO_TIME_TO_RESPAWN);
    }
    break;

  case UFO_EXPLODING:
    if (!clock.hasTimedOut())
      break;
  
    state = UFO_SHOWING_SCORE;
    clock.reset(2);
    addToScore(scoreValue);
    break;
  
  case UFO_SHOWING_SCORE:
    if (!clock.hasTimedOut())
      break;
  
    state = UFO_AWAY;
    clock.reset(UFO_TIME_TO_RESPAWN);
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
  case UFO_ALIVE:
  {
    const SDL_Rect src = {0, 0, 16, 8},
                   dst = {(int)roundf(x), UFO_Y, 2 * TILE, TILE};
    SDL_RenderCopy(ren, atlas, &src, &dst);
    break;
  }
  case UFO_EXPLODING:
  {
    const SDL_Rect src = {0, 0, 24, 8},
                   dst = {(int)roundf(x) - 4, UFO_Y, 3 * TILE, TILE};
    SDL_RenderCopy(ren, atlas, &src, &dst);
    break;
  }
  case UFO_SHOWING_SCORE:
    // NAGE::renderText(xi, Y, "%3d", scoreValue);
    break;

  default:
    break;
  }
}

UFO ufo;
