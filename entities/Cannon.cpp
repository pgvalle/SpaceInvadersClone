#include "Cannon.h"
#include "app/App.h"
#include "defines.h"


#define Y (HEIGHT - 5 * TILE)

Cannon::Cannon()
{
  state = ALIVE;
  x = 14;
}

Shot Cannon::shoot() const
{
  return {x + 9, Y, -4};
}

bool Cannon::checkAndProcessHit(const SDL_Rect &hitbox)
{
  const SDL_Rect cannon = {x, Y, 16, 8};

  const bool hit = SDL_HasIntersection(&hitbox, &cannon);
  if (hit)
  {
    state = DYING;
    deathFrame = 0;
    clock1.reset(100); // animation frame time
    clock2.reset(2000); // dying state time
  }

  return hit;
}

void Cannon::update()
{
  switch (state)
  {
  case ALIVE:
    if (isKeyPressed(SDL_SCANCODE_LEFT))
    {
      x -= (x > 14 ? 2 : 0);
    }
    if (isKeyPressed(SDL_SCANCODE_RIGHT))
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

void Cannon::render() const
{
  switch (state)
  {
  case ALIVE:
    renderClip(x, Y, {0, 8, 16, 8});
    break;
  case DYING:
    renderClip(x, Y, {16 + 16 * deathFrame, 8, 16, 8});
    break;
  case DEAD:
    break;
  }
}
