#include "Horde.h"
#include "app/App.h"
#include "defines.h"


Horde::Horde()
{
  state = STARTING;
  delayer.reset(16);
}

Shot Horde::shoot() const
{
  const int ri = rand() % invaders.size();
  const Invader& invader = invaders[ri];

  return {invader.x + 5, invader.y + 8, 3};
}

Explosion Horde::checkAndProcessHit(const SDL_Rect& hitbox)
{
  for (int i = 0; i < invaders.size(); i++)
  {
    const Invader invader = invaders[i];
    const SDL_Rect invaderHB = invader.getHitbox();
    if (SDL_HasIntersection(&hitbox, &invaderHB))
    {
      // delete invader from list
      invaders.erase(invaders.begin() + i--);
      // update horde movement delay
      const Uint32 newDelay = invaders.size() * 16;
      delayer.reset(newDelay);
      // sum score value
      app->score += invader.getScoreValue();
      // return explosion to be processed
      return Explosion(invader.x, invader.y, 100, {24, 32, 13, 8});
    }
  }

  // invalid explosion. No collision happened
  return Explosion(0, 0, 0, {0, 0, 0, 0});
}

void Horde::update()
{
  switch (state)
  {
  case STARTING:
    delayer.update();
    if (!delayer.hasTimedOut()) break;
    // Done. Now start moving
    if (invaders.size() == 55)
    {
      state = MOVING;
      xVel = 2; // going right
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
    // update delayer
    delayer.update();
    if (!delayer.hasTimedOut()) break;
    delayer.reset();
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
    // move invaders now
    for (Invader &invader : invaders)
    {
      invader.move(xVel, yVel);
    }

    break;
  }
}

void Horde::render() const
{
  for (const Invader &invader : invaders)
  {
    invader.render();
  }
}
