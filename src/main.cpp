#include "SIC.h"
#include "screens.h"

int main(int argc, char **argv)
{
  srand(time(NULL));

  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  sic.init();
  sic.loop();
  sic.quit();

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
