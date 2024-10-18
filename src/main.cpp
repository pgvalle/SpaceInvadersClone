#include "SIC.h"
#include "screens.h"

int main(int argc, char **argv)
{
  srand(time(NULL));

  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();

  _sic = new SIC;
  sic.init();
  sic.loop();
  sic.quit();
  delete _sic;

  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}
