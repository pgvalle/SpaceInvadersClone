#include "App.h"
#include "scenes/PlayScene.h"

int main(int argc, char **argv)
{
  App::init();

  App::setTitle("Space Invaders");
  App::setIcon("res/icon.bmp");
  App::setDimensions(28, 32);
  App::setFPS(60);
  
  App::loadAtlas("res/atlas.png");
  App::loadFont("res/ps2p.ttf");

  UIScene *ui = new UIScene();
  App::run(new PlayScene(ui));
  delete ui;

  App::quit();

  return 0;
}
