#include "SIC.h"

int main(int argc, char **argv)
{
  srand(time(nullptr));
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  g = new SIC();
  g->loop();

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return 0;
}
