#include "app/App.h"
#include "PlayScene.h"


PlayScene::PlayScene()
{
  state = STARTING;
  delayer.reset(3000);

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
  case PLAYING:
    horde.update();
    cannon->update();

    break;
  default:
    break;
  }
  ufo.update();
  horde.update();
}

void PlayScene::render()
{
  ufo.render();
  horde.render();
}
