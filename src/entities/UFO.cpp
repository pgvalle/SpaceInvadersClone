#include "UFO.h"
#include "Events.h"

#include <NAGE.h>
#include <stdio.h>

#define Y (5 * 8)

UFO::UFO()
{
  // start at away state
  state = AWAY;
  clock.reset(10); // spawn
}

bool UFO::checkAndProcessHit(const SDL_Rect &hitbox)
{
  // not even there to be hit
  if (state != ALIVE)
  {
    return false;
  }

  const SDL_Rect ufoHB = {x + 4, Y, 16, 8};
  const bool hit = SDL_HasIntersection(&hitbox, &ufoHB);
  if (hit)
  {
    // transition to dying1 state
    state = DYING1;
    clock.reset(0.3);
  }

  return hit;
}

void UFO::update(float delta)
{
  switch (state)
  {
    case AWAY:
      clock.update(delta);
      if (clock.hasTimedOut())
      {
        state = ALIVE;
        // randomly choose a corner
        const bool left = rand() % 2;
        x = left ? 8 : (24 * 8);
        xVel = left ? 1 : -1;
        // randomly choose a score value
        scoreValue = 100;

        clock.reset(0.06);
      }
      break;
    case ALIVE:
      clock.update(delta);
      if (clock.hasTimedOut())
      {
        x += xVel;
        if (x < 8 || x > 24 * 8) // out of bounds
        {
          // transition to away state
          state = AWAY;
          clock.reset(10);
        }
      }
      break;
    case DYING1:
      // animation frame should change
      clock.update(delta);
      if (clock.hasTimedOut())
      {
        state = DYING2; // now we show it's score value
        clock.reset(2);
        // sum score value
        NAGE::pushEvent<int>(SCORE_UPDATE_EVENT, scoreValue);
      }
      break;
    case DYING2:
      clock.update(delta);
      if (clock.hasTimedOut())
      {
        // transition to away state
        state = AWAY;
        clock.reset(10);
      }
      break;
    default:
      break;
  }
}

void UFO::render(SDL_Renderer *renderer) const
{
  switch (state)
  {
    case ALIVE:
      NAGE::renderTile(x, Y, 0);
      NAGE::setFlip(SDL_FLIP_HORIZONTAL);
      NAGE::renderTile(x + 8, Y, 0);
      NAGE::setFlip(SDL_FLIP_NONE);
      break;
    case DYING1:
      NAGE::renderTile(x, Y, 1);
      NAGE::renderTile(x + 8, Y, 2);
      NAGE::renderTile(x + 16, Y, 3);
      break;
    case DYING2:
      NAGE::renderText(x, Y, "%3d", scoreValue);
      break;
    default:
      break;
  }
}
