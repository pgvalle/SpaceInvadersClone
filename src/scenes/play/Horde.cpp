#include "Horde.h"


Horde::Horde()
{
  state = STARTING;
  delayer.reset(55 * 16);
}

bool Horde::isDestroyed()
{
  return invaders.empty();
}

void Horde::forceDelayOnHit()
{
  // timeout will decrease with invader amount decreasing
  const Uint32 newTimeout = invaders.size() * 16;
  delayer.reset(newTimeout);
}

void Horde::update()
{
  switch (state)
  {
  case STARTING:
    if (invaders.size() == 55) // done. Now start moving
    {
      state = MOVING;
      xVel = 2;
      xStepCount = 8;
    }
    else
    {
      const int col = invaders.size() % 11;
      const int row = 4 - invaders.size() / 11;
      invaders.push_back(Invader(col, row));
    }

    break;
  case MOVING:
    delayer.update();
    if (delayer.hasTimedOut())
    {
      int yVel = 0;
      if (++xStepCount == 17) // move down and to the other direction
      {
        xVel = -xVel;
        yVel = 8;
        xStepCount = 0;
      }

      for (Invader &invader : invaders)
      {
        invader.move(xVel, yVel);
      }

      delayer.reset();
    }

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
