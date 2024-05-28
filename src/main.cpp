#include <NAGE.h>
#include "scenes/PlayScene.h"

int main(int argc, char **argv)
{
  NAGE::init();

  NAGE::setTitle("Space Invaders");
  NAGE::setIcon("res/icon.bmp");
  NAGE::setDimensions(28, 32);
  NAGE::setFPS(60);
  
  NAGE::loadAtlas("res/atlas.png");
  NAGE::loadFont("res/ps2p.ttf");

  UIScene *ui = new UIScene();
  NAGE::run(new PlayScene(ui));
  delete ui;

  NAGE::quit();

  return 0;
}
