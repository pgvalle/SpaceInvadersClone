#include "UFO.h"
#include "app/App.h"
#include "defines.h"

#include <stdio.h>


#define Y (5 * TILE)

UFO::UFO()
{
  // start at away state
  state = AWAY;
  clock.reset(10000); // spawn
}

bool UFO::checkAndProcessHit(const SDL_Rect &hitbox)
{
  const SDL_Rect ufoHB = {x + 4, Y, 16, 8};
  const bool hit = SDL_HasIntersection(&hitbox, &ufoHB);
  if (hit)
  {
    // transition to dying1 state
    state = DYING1;
    clock.reset(300);
  }

  return hit && state == ALIVE;
}

void UFO::update()
{
  switch (state)
  {
  case AWAY:
    clock.update();
    if (clock.hasTimedOut())
    {
      // randomly choose a corner
      const bool left = rand() % 2;
      x = left ? TILE : (24 * TILE);
      xVel = left ? 1 : -1;
      // randomly choose a score value
      scoreValue = 100;

      clock.reset(60);
    }
    break;
  case ALIVE:
    clock.update();
    if (clock.hasTimedOut())
    {
      x += xVel;
      if (x < TILE || x > 24 * TILE) // out of bounds
      {
        // transition to away state
        state = AWAY;
        clock.reset(10000);
      }
    }
    break;
  case DYING1:
    // animation frame should change
    clock.update();
    if (clock.hasTimedOut())
    {
      state = DYING2; // now we show it's score value
      clock.reset(2000);
      // sum score value
      app->score += scoreValue;
    }
    break;
  case DYING2:
    clock.update();
    if (clock.hasTimedOut())
    {
      // transition to away state
      state = AWAY;
      clock.reset(10000);
    }
    break;
  default:
    break;
  }
}

void UFO::render() const
{
  switch (state)
  {
  case ALIVE:
    app->renderClip(x, Y, {0, 0, 24, 8});
    break;
  case DYING1:
    app->renderClip(x, Y, {24, 0, 24, 8});
    break;
  case DYING2:
    char scoreFmt[4];
    sprintf(scoreFmt, "%3d", scoreValue);
    app->renderText(x, Y, scoreFmt, {0xD8, 0x20, 0x20});
    break;
  default:
    break;
  }
}
