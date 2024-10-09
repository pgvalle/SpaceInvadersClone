#include "Cannon.h"
#include "SIC.h"

#define CANNON_Y (HEIGHT - 5 * TILE)

Cannon::Cannon()
{
  state = ALIVE;
  x = 14;
}

void Cannon::onHit()
{
  state = DYING;
  deathFrame = 0;
  clock1.reset(100); // animation frame time
  clock2.reset(2000); // dying state time
}

SDL_Rect Cannon::getHitbox() const
{
  return {x, CANNON_Y, 16, 8};
}

void Cannon::onUpdate(float dt)
{
  switch (state)
  {
    case DYING: {
      // animation frame update
      clock1.update(dt);
      if (clock1.hasTimedOut())
      {
        deathFrame = !deathFrame;
        clock1.reset();
      }
      // state duration timeout update
      clock2.update(dt);
      if (clock2.hasTimedOut())
        state = DEAD;
      break;
    }
    case DEAD:
      break;
  }
}

void Cannon::onRender() const
{
  switch (state)
  {
    case ALIVE: {
      const SDL_Rect src = {0, 8, 16, 8};
      const SDL_Rect dst = {x, CANNON_Y, 16, 8};
      SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
      break;
    }
    case DYING: {
      const SDL_Rect src = {16 + 16 * deathFrame, 8, 16, 8};
      const SDL_Rect dst = {x, CANNON_Y, 16, 8};
      SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
      break;
    }
    default:
      break;
  }
}

void Cannon::move(float vx, float dt)
{
  x += vx * dt;
}
