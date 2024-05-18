#include "UFO.h"
#include "App.h"

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
    return false;

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
        SDL_UserEvent event;
        // event.type = SDL_USEREVENT;
        // event.code = SCORE_UPDATE_EVENT;
        // event.data1 = &scoreValue;
        // SDL_PushEvent((SDL_Event *)&event);
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
      App::renderTile(x, Y, 0);
      App::setFlip(SDL_FLIP_HORIZONTAL);
      App::renderTile(x + 8, Y, 0);
      App::setFlip(SDL_FLIP_NONE);
      break;
    case DYING1:
      App::renderTile(x, Y, 1);
      App::renderTile(x + 8, Y, 2);
      App::renderTile(x + 16, Y, 3);
      break;
    case DYING2:
      App::renderText(x, Y, "%3d", scoreValue);
      break;
    default:
      break;
  }
}
