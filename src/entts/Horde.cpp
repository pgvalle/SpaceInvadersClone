#include "Horde.h"
#include "Explosion.h"
#include "SIC.h"

#define HORDE_LLIMIT (TILE)
#define HORDE_RLIMIT (WIDTH - 3 * TILE)

Horde::Horde()
{
  clock.reset(0.016);
  populating = true;
}

Explosion *Horde::onHit(const SDL_Rect &rect)
{
  for (int i = 0; i < invaders.size(); i++)
  {
    Explosion *e = invaders[i].onHit(rect);
    if (e)
    {
      invaders.erase(invaders.begin() + i);
      return e;
    }
  }

  return nullptr;
}

SDL_Rect Horde::getHitbox() const
{
  return {0, 0, 0, 0};
}

void Horde::onTick(float dt)
{
}

void Horde::onUpdate(float dt)
{
  clock.update(dt);
  if (!clock.hasTimedOut())
    return;

  if (populating) // still populating
  {   
    const int count = invaders.size();
    if (count == 55) // done populating
    {
      clock.reset(invaders.size() * 0.016);
      populating = false;
      x = 7;
      vx = 2; // start going right
      return;
    }

    // still populating
    clock.reset();

    // start from the bottom-left. Iterate line first
    const int row = 4 - count / 11;
    const int col = count % 11;
    invaders.push_back(Invader(row, col));
    return;
  }

  // not populating
  clock.reset(invaders.size() * 0.016);

  // check if it's time to turn around
  int vy = 0;
  for (Invader &inv : invaders)
  {
    const SDL_Rect hb = inv.getHitbox();
    if (hb.x < HORDE_LLIMIT || hb.x > HORDE_RLIMIT) // turn around
    {
      vx = -vx;
      vy = 8;
      break;
    }
  }

  // move invaders accordingly
  for (Invader &inv : invaders)
    inv.move(vx, vy);
}

void Horde::onRender() const
{
  for (const Invader &inv : invaders)
    inv.onRender();
}
