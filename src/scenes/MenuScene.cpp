#include "app/App.h"
#include "MenuScene.h"
#include "PlayScene.h"

#include <string.h>


MenuScene::MenuScene()
{
  clock.reset(2000);
}

MenuScene::~MenuScene()
{
  
}


void MenuScene::processEvent()
{

}

void MenuScene::update()
{
  clock.update();
  if (clock.hasTimedOut())
  {
    changeScene(new PlayScene());
  }
}

void MenuScene::render()
{
}
