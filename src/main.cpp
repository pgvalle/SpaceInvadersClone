#include "App.h"
#include "scenes/PlayScene.h"

int main(int argc, char **argv)
{
  NAGE::init();

  NAGE::setAppTitle("Space Invaders");
  NAGE::setAppIcon("res/icon.bmp");
  NAGE::setAppDimensions(28, 32);
  NAGE::setAppFPS(60);
  
  NAGE::loadAtlas("res/atlas.png");
  NAGE::loadFont("res/ps2p.ttf");

  UIScene *ui = new UIScene();
  NAGE::run(new PlayScene(ui));
  delete ui;

  NAGE::quit();

  return 0;
}
