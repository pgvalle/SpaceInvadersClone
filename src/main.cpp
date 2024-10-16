#include "SIC.h"

int main(int argc, char **argv)
{
  srand(time(NULL)); // so that we have random numbers

  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  sic = new SIC;
  sic->init();
  sic->loop();
  sic->quit();

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
