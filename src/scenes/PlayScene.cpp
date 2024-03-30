#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"


PlayScene::PlayScene()
{
  state = STARTING;
  delayer.reset(3000);

  ufo = nullptr;
  ufoClock.reset(5000);

  cannon = nullptr;
  cannonLives = 3;
}

PlayScene::~PlayScene()
{
  
}


void PlayScene::processEvent()
{
}

void PlayScene::update()
{
  switch (state)
  {
  case STARTING:
    horde.update();

    delayer.update();
    if (delayer.hasTimedOut())
    {
      state = PLAYING;
      cannon = new Cannon();
    }

    break; 
  case PLAYING: {
    ufoClock.update();
    if (ufoClock.hasTimedOut())
    {
      ufo = new UFO();
      ufoClock.reset(10000);
    }

    if (ufo)
    {
      ufo->update();
    }

    horde.update();
    cannon->update();
    const bool hit = cannon->checkAndProcessHit({WIDTH - 5 * TILE, HEIGHT - 5 * TILE, 20, 8});
    if (hit)
    {
      state = DELAYING;
    }

    break; }
  case DELAYING:
    if (!cannon)
    {
      delayer.update();
      if (delayer.hasTimedOut())
      {
        state = PLAYING;
        cannon = new Cannon();
      }
    }
    else
    {
      cannon->update();
      if (cannon->isDead())
      {
        delete cannon;
        cannon = nullptr;
        delayer.reset(5000);
        cannonLives--;
      }
    }

    break;
  }
}

void PlayScene::render()
{
  //ufo->render();
  horde.render();

  if (cannon)
  {
    cannon->render();
  }

  if (ufo)
  {
    ufo->render();
  }

  // render lifes
  char livesFmt[] = {'0' + cannonLives, '\0'};
  app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);
}
