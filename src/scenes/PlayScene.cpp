#include "app/App.h"
#include "PlayScene.h"


PlayScene::PlayScene()
{
  
}

PlayScene::~PlayScene()
{
  
}


void PlayScene::processEvent()
{
}

void PlayScene::update()
{
  ufo.update();
  horde.update();
}

void PlayScene::render()
{
  ufo.render();
  horde.render();
}
