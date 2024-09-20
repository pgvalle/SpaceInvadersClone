#include "horde.h"
#include "globals.h"

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
    const SDL_Rect invaderRect = invaders[i].getRect();
    if (!SDL_HasIntersection(&rect, &invaderRect))
      continue;

    Explosion *e = new Explosion(invaders[i].x, invaders[i].y, 0.2);
    e->clip = {28, 16, 13, 8};

    addToScore(invaders[i].getScoreValue());

    invaders.erase(invaders.begin() + i);

    return e;
  }

  return nullptr;
}

void Horde::update()
{
  clock.update();

  if (!clock.hasTimedOut())
    return;

  if (populating && invaders.size() < 55) // still populating
  {
    clock.reset();
    
    const int count = invaders.size();
    // start from the bottom-left. Iterate line first
    const int row = 4 - count / 11;
    const int col = count % 11;
    invaders.push_back(Invader(row, col));
    return;
  }

  if (populating) // now populated
  {
    clock.reset(invaders.size() * 0.016);
    populating = false;
    x = 7;
    vx = 2; // start going right
    return;
  }

  // time to move horde
  clock.reset(invaders.size() * 0.016);

  int vy = 0;
  for (Invader &i : invaders)
  {
    const SDL_Rect rect = i.getRect();
    if (rect.x < HORDE_LLIMIT || rect.x > HORDE_RLIMIT)
    {
      vx = -vx;
      vy = 8;
      break;
    }
  }

  for (Invader &i : invaders)
    i.move(vx, vy);
}

void Horde::render() const
{
  for (const Invader &i : invaders)
    i.render();
}

Horde horde;
