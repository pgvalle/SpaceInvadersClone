#include "../core.h"

#include <string.h>


static const char* TEXT = "PLAY\nSPACE INVADERS\n=? MYSTERY\n=30 POINTS\n=20 POINTS\n=10 POINTS";


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
