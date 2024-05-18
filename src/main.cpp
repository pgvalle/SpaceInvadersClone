#include "App.h"

#include "scenes/UIScene.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <ctime>

int main(int argc, char **argv)
{
  App::init();

  App::setTitle("Space Invaders");
  App::setIcon("res/icon.bmp");
  App::setDimensions(28, 32);
  App::setFPS(60);
  
  App::loadAtlas("res/atlas.png");
  App::loadFont("res/ps2p.ttf");

  App::run(new UIScene());

  App::quit();

  return 0;
}
