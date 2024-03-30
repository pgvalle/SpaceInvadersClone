#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"


PlayScene::PlayScene()
{
  state = STARTING;
  delayer.reset(3000);

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
    ufo.update();
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
        delayer.reset(2000);
      }
    }

    break;
  }
}

void PlayScene::render()
{
  ufo.render();
  horde.render();

  if (cannon)
  {
    cannon->render();
  }
}
