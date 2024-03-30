#include "UFO.h"
#include "app/App.h"
#include "defines.h"

UFO::UFO()
{
  state = HIDDEN;

  clock.reset(5000); // spawn
  x = -10000;
  xVel = 0;
  scoreValue = 100;
}

void UFO::process_strike()
{
  state = DYING;
  clock.reset(120);
}

void UFO::update()
{
  switch (state)
  {
  case HIDDEN:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = VISIBLE;
      // pick a random side to appear
      const bool left = rand() % 2;
      x = left ? 8 : 200;
      xVel = left ? 1 : -1;
      // will be used as motion clock
      clock.reset(60);
    }

    break;
  case VISIBLE:
    clock.update();
    if (clock.hasTimedOut())
    {
      x += xVel;
      if (x < 8 || x > 200)
      {
        state = HIDDEN;
        clock.reset(5000);
      }
    }

    break;
  case DYING:
    // animation frame should change
    clock.update();
    if (clock.hasTimedOut())
    {
      state = DEAD; // now we show it's score value
      clock.reset(2000);
    }

    break;
  case DEAD:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = AFTERLIFE;
    }

    break;
  case AFTERLIFE:
    break;
  }
}

void UFO::render()
{
  switch (state)
  {
  case HIDDEN:
    break;
  case VISIBLE:
    app->renderClip(x, 5 * TILE, {4, 0, 16, 8});
    break;
  case DYING:
    app->renderClip(x - 4, 5 * TILE, {24, 0, 24, 8});
    break;
  case DEAD:
    
    break;
  case AFTERLIFE:
    break;
  }
}
