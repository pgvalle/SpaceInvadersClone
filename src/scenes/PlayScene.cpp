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
  horde.update();
}

void PlayScene::render()
{
  horde.render();
}
