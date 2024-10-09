#include "Cannon.h"
#include "SIC.h"

#define CANNON_VX 200
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
  return {(int)round(x), CANNON_Y, 13, 8};
}

void Cannon::onUpdate(float dt)
{
  switch (state)
  {
    case ALIVE: {
      const Uint8 *keys = SDL_GetKeyboardState(NULL);
      if (keys[SDL_SCANCODE_LEFT])
        x -= dt * (x > 14 ? CANNON_VX : 0);
      if (keys[SDL_SCANCODE_RIGHT])
        x += dt * (x < WIDTH - 31 ? CANNON_VX : 0);
      break;
    }
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
      const SDL_Rect src = {0, 8, 13, 8};
      const SDL_Rect dst = {(int)round(x), CANNON_Y, 13, 8};
      SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
      break;
    }
    case DYING: {
      const SDL_Rect src = {13 + 16 * deathFrame, 8, 16, 8};
      const SDL_Rect dst = {(int)round(x) - 1, CANNON_Y, 16, 8};
      SDL_RenderCopy(g->ren, g->atlas, &src, &dst);
      break;
    }
    default:
      break;
  }
}

Shot *Cannon::shoot() const
{
  Shot *s = new Shot;
  s->x = x;
  s->y = CANNON_Y;
  s->vy = -80;
  s->clip = {0, 0, 20, 20};
  return s;
}
