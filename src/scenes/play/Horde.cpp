#include "Horde.h"
#include "defines.h"


Horde::Horde()
{
  state = STARTING;
  delayer.reset(16);
}

bool Horde::isDestroyed()
{
  return invaders.empty();
}

Explosion Horde::checkAndProcessInvaderHit(const SDL_Rect& hitbox)
{
  for (Invader &invader : invaders)
  {
    const SDL_Rect invaderHitbox = invader.getHitbox();
    if (SDL_HasIntersection(&hitbox, &invaderHitbox))
    {
      // set delay on hit
      const Uint32 newTimeout = invaders.size() * 16;
      delayer.reset(newTimeout);
      return Explosion(invader.x, invader.y, 100, {32, 24, 13, 8});
    }
  }

  return Explosion(0, 0, 0, {0, 0, 0, 0});
}

void Horde::update()
{
  switch (state)
  {
  case STARTING:
    delayer.update();
    if (!delayer.hasTimedOut()) break;

    if (invaders.size() == 55) // done. Now start moving
    {
      state = MOVING;
      xVel = 2;
      delayer.reset(invaders.size() * 16);
    }
    else
    {
      const int col = invaders.size() % 11;
      const int row = 4 - invaders.size() / 11;
      invaders.push_back(Invader(col, row));
      delayer.reset();
    }

    break;
  case MOVING:
    delayer.update();
    if (!delayer.hasTimedOut()) break;

    // check if it's time to change direction
    int yVel = 0;
    for (const Invader &invader : invaders)
    {
      if (12 > invader.x || invader.x > WIDTH - 24)
      {
        xVel = -xVel;
        yVel = 8;
        break;
      }
    }

    for (Invader &invader : invaders)
    {
      invader.move(xVel, yVel);
    }

    delayer.reset();

    break;
  }
}

void Horde::render()
{
  for (Invader &invader : invaders)
  {
    invader.render();
  }
}
