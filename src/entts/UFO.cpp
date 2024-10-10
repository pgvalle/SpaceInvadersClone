#include "UFO.h"
#include "Explosion.h"
#include "SIC.h"

#define UFO_Y (5 * TILE)
#define UFO_LLIMIT (TILE)
#define UFO_RLIMIT (24 * TILE)

#define UFO_DX 1.2

#define UFO_TICKS_SHOWING_SCORE 60
#define UFO_TICKS_EXPLODING 30
#define UFO_TICKS_TO_RESPAWN 300

UFO::UFO()
{
  state = UFO_AWAY;
  ticker.reset(UFO_TICKS_TO_RESPAWN); // spawn after 10-15s
}

Explosion *UFO::collisionCallback()
{
  state = UFO_EXPLODING;
  ticker.reset(UFO_TICKS_EXPLODING);

  Explosion *e = new Explosion;
  e->x = x - 4; // offset bc explosion sprite width is 8 pixels bigger
  e->y = UFO_Y;
  e->clip = {16, 0, 24, 8};
  e->lifespan.reset(UFO_TICKS_EXPLODING);
  return e;
}

SDL_Rect UFO::getCollider() const
{
  return {(int)round(x), UFO_Y, 16, 8};
}

void UFO::tick()
{
  ticker.tick();
  if (!ticker.hasFinished())
    return;

  switch (state)
  {
  case UFO_AWAY:
  {
    state = UFO_ALIVE;
    ticker.reset(1);
    // randomly choose a corner to spawn at: left or right
    bool left = rand() % 2;
    x = left ? UFO_LLIMIT : UFO_RLIMIT;
    dx = left ? UFO_DX : -UFO_DX; // velocity = DX / ticktime
    break;
  }
  case UFO_ALIVE:
  {
    const int newX = round(x += dx);
    if (UFO_LLIMIT <= newX && newX <= UFO_RLIMIT)
      break;
    // out of bounds
    state = UFO_AWAY;
    ticker.reset(UFO_TICKS_TO_RESPAWN);
    break;
  }
  case UFO_EXPLODING:
  {
    state = UFO_SHOWING_SCORE;
    ticker.reset(UFO_TICKS_SHOWING_SCORE);
    sic->score += 100; // TODO: generate random score
    break;
  }
  case UFO_SHOWING_SCORE:
  {
    state = UFO_AWAY;
    ticker.reset(UFO_TICKS_TO_RESPAWN);
    break;
  }
  default:
    break;
  }
}

void UFO::draw() const
{
  SDL_SetRenderDrawColor(sic->renderer, 255, 0, 0, 255); // red

  switch (state)
  {
  case UFO_ALIVE:
  {
    const SDL_Rect src = {0, 0, 16, 8};
    const SDL_Rect dst = {(int)round(x), UFO_Y, 2 * TILE, TILE};
    SDL_RenderCopy(sic->renderer, sic->atlas, &src, &dst);
    break;
  }
  case UFO_SHOWING_SCORE:
  {
    FC_DrawColor(sic->font, sic->renderer, 0, 0, {255, 0, 0, 255}, "%d", score);
    break;
  }
  default:
    break;
  }
}
