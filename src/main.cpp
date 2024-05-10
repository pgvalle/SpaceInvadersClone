#include "App.h"

#include "scenes/MainScene.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <ctime>

int main(int argc, char **argv) {
  Config conf;
  conf.atlasPath = "res/atlas.png";
  conf.fontPath = "res/ps2p.ttf";
  conf.fps = 30;
  conf.wTiles = 28;
  conf.hTiles = 32;
  initialize(conf);
  run(new MainScene());
  terminate();

  return 0;
}
