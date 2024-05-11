#include "App.h"

#include "scenes/MainScene.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <ctime>

int main(int argc, char **argv)
{
  initialize({.atlasPath = "res/atlas.png",
              .fontPath = "res/ps2p.ttf",
              .fps = 60,
              .wTiles = 28,
              .hTiles = 32});
  run(new MainScene());
  terminate();

  return 0;
}
