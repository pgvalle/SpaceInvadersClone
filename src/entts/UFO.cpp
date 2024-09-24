#include "UFO.h"
#include "glob.h"

#define UFO_Y (5 * TILE)
#define UFO_LLIMIT (TILE)
#define UFO_RLIMIT (24 * TILE)

#define UFO_SPEED 40

#define UFO_TIME_SHOWING_SCORE 2
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

  const SDL_Rect ufoRect = {(int)round(x), UFO_Y, 2 * TILE, TILE};

  if (!SDL_HasIntersection(&rect, &ufoRect))
    return nullptr;

  state = UFO_EXPLODING;
  clock.reset(UFO_TIME_EXPLODING);

  Explosion *e = new Explosion;
  e->x = x - 4; // offset bc explosion sprite width is 8 pixels bigger
  e->y = UFO_Y;
  e->clip = {16, 0, 24, 8};
  e.lifespan.reset(UFO_TIME_EXPLODING);

  return e;
}

void UFO::update(float dt)
{
  clock.update(dt);

  // States other than UFO_ALIVE depend on clock timing out. So if it didn't timeout, just don't update
  if (state != UFO_ALIVE && !clock.hasTimedOut())
    return;

  switch (state)
  {
  case UFO_AWAY:
    state = UFO_ALIVE;
    clock.reset(1);  // we don't care about clock in UFO_ALIVE

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
    x += vx*dt;

    if (UFO_LLIMIT > round(x) && round(x) > UFO_RLIMIT)
    {
      state = UFO_AWAY;
      clock.reset(UFO_TIME_TO_RESPAWN);
    }
    break;

  case UFO_EXPLODING:
    state = UFO_SHOWING_SCORE;
    clock.reset(UFO_TIME_SHOWING_SCORE);
    g->add2Score(100);
    break;

  case UFO_SHOWING_SCORE:
    state = UFO_AWAY;
    clock.reset(UFO_TIME_TO_RESPAWN);
    break;

  default:
    break;
  }  
}

void UFO::render() const
{
  SDL_SetRenderDrawColor(g->ren, 255, 0, 0, 255);

  switch (state)
  {
  case UFO_ALIVE:
  {
    const SDL_Rect src = {0, 0, 16, 8},
                   dst = {(int)round(x), UFO_Y, 2 * TILE, TILE};
    SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
    break;
  }
  case UFO_EXPLODING:
  {
    const SDL_Rect src = {0, 0, 24, 8},
                   dst = {(int)round(x) - 4, UFO_Y, 3 * TILE, TILE};
    SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
    break;
  }
  case UFO_SHOWING_SCORE:
    // NAGE::renderText(xi, Y, "%3d", scoreValue);
    break;

  default:
    break;
  }
}
