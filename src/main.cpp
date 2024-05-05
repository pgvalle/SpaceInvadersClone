#include "App.h"

#include "scenes/MainScene.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <ctime>

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  srand(time(nullptr));

  run(new MainScene());

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return 0;
}
