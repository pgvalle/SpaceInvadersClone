#include "UFO.h"
#include "app/App.h"
#include "defines.h"

#include <stdio.h>


#define Y (5 * TILE)

UFO::UFO()
{
  state = ALIVE;
  clock.reset(60); // spawn

  // randomly choose a corner
  const bool left = rand() % 2;
  x = left ? TILE : (24 * TILE);
  xVel = left ? 1 : -1;
  // randomly choose a score value
  scoreValue = 100;
}

bool UFO::checkAndProcessHit(const SDL_Rect &hitbox)
{
  const SDL_Rect ufo = {x + 4, Y, 16, 8};

  const bool hit = SDL_HasIntersection(&hitbox, &ufo);
  if (hit)
  {
    state = DYING1;
    clock.reset(300);
  }

  return hit;
}

void UFO::update()
{
  switch (state)
  {
  case ALIVE:
    clock.update();
    if (!clock.hasTimedOut()) break;

    x += xVel;
    if (x < TILE || x > 24 * TILE)
    {
      state = DEAD;
    }

    break;
  case DYING1:
    // animation frame should change
    clock.update();
    if (!clock.hasTimedOut()) break;

    state = DYING2; // now we show it's score value
    clock.reset(2000);
    // sum score value
    app->score += scoreValue;

    break;
  case DYING2:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = DEAD;
    }

    break;
  case DEAD:
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
  case DEAD:
    break;
  }
}
