#include "UFO.h"
#include "glob.h"

#define UFO_Y (5 * TILE)
#define UFO_LLIMIT (TILE)
#define UFO_RLIMIT (24 * TILE)

#define UFO_SPEED 40

#define UFO_TIME_SHOWING_SCORE 2
#define UFO_TIME_EXPLODING 1
#define UFO_TIME_TO_RESPAWN Timer::getRandomTime(10, 15)

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
  e->lifespan.reset(UFO_TIME_EXPLODING);

  return e;
}

void UFO::update(float dt)
{
  clock.update(dt);
  if (!clock.hasTimedOut())
    return;

  switch (state)
  {
  case UFO_AWAY: {
    state = UFO_ALIVE;
    clock.reset(1e-5);  // we don't care about clock in UFO_ALIVE
    // randomly choose a corner to spawn in: left or right
    bool left = rand() % 2;
    x = left ? UFO_LLIMIT : UFO_RLIMIT;
    vx = left ? UFO_SPEED : -UFO_SPEED;
    break;
  }
  case UFO_ALIVE:
    x += vx*dt;
    if (UFO_LLIMIT > round(x) || round(x) > UFO_RLIMIT)
    {
      state = UFO_AWAY;
      clock.reset(UFO_TIME_TO_RESPAWN);
    }
    break;

  case UFO_EXPLODING:
    state = UFO_SHOWING_SCORE;
    clock.reset(UFO_TIME_SHOWING_SCORE);
    score = 100;
    g->add2Score(score);
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
  case UFO_SHOWING_SCORE:
    FC_DrawColor(g->font, g->ren, 0, 0, {255, 0, 0, 255}, "%03d", score);
    break;

  default:
    break;
  }
}
