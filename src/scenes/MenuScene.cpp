#include "../core.h"

#include <string.h>


MenuScene::MenuScene()
{
  clock.reset(5000);
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
    app->changeScene(new PlayScene());
  }
}

void MenuScene::render()
{
}
