#include "Horde.h"


Horde::Horde()
{
  frozen_timer.reset(55 * 16);
  state = STARTING;
}

void Horde::freezeTemporarily()
{
  // timeout will decrease with invader amount decreasing
  const Uint32 newTimeout = invaders.size() * 16;
  frozen_timer.reset(newTimeout);
}

bool Horde::isFinished()
{
  return invaders.empty();
}

void Horde::updateStarting()
{
  if (invaders.size() == 55)
  {
    // done. Now start moving
    state = State::MOVING;
    xrel = 2;
    xrelCount = 8;
  }
  else
  {
    const int row = 4 - invaders.size() / 11;
    const int col = invaders.size() % 11;
    invaders.push_back(Invader(col, row));
  }
}

void Horde::updateMoving()
{
  frozen_timer.update();
  if (frozen_timer.hasTimedOut())
  {
    int yrel = 0;
    if (++xrelCount == 17)
    {
      xrel = -xrel;
      yrel = 8;
      xrelCount = 0;
    }

    for (Invader &invader : invaders)
    {
      invader.move(xrel, yrel);
    }

    frozen_timer.reset();
  }
}

void Horde::update()
{
  switch (state)
  {
  case STARTING:
    updateStarting();
    break;
  case MOVING:
    updateMoving();
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
