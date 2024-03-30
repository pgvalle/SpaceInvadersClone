#include "Cannon.h"
#include "app/App.h"
#include "defines.h"


Cannon::Cannon()
{
  state = ALIVE;
  x = 14;
}

bool Cannon::isDead()
{
  return state == DEAD;
}

void Cannon::checkAndProcessHit(const SDL_Rect &hitbox)
{
  const SDL_Rect cannonHitbox = {x, HEIGHT - 5 * TILE, 16, 8};
  if (SDL_HasIntersection(&hitbox, &cannonHitbox))
  {
    state = DYING;
    deathFrame = 0;
    clock1.reset(100); // animation frame
    clock2.reset(2000); // duration of dying state
  }
}

void Cannon::update()
{
  switch (state)
  {
  case ALIVE:
    if (app->isKeyPressed(SDL_SCANCODE_LEFT))
    {
      x -= (x > 14 ? 2 : 0);
    }
    if (app->isKeyPressed(SDL_SCANCODE_RIGHT))
    {
      x += (x < WIDTH - 31 ? 2 : 0);
    }

    break;
  case DYING:
    // animation frame update
    clock1.update();
    if (clock1.hasTimedOut())
    {
      deathFrame = !deathFrame;
      clock1.reset();
    }
    // state duration timeout update
    clock2.update();
    if (clock2.hasTimedOut())
    {
      state = DEAD;
    }

    break;
  case DEAD:
    break;
  }
}

void Cannon::render()
{
  switch (state)
  {
  case ALIVE:
    app->renderClip(x, HEIGHT - 5 * TILE, {0, 8, 16, 8});
    break;
  
  default:
    break;
  }
}
