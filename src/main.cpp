#include "common.h"

void loop();

int main(int argc, char **argv)
{
  srand(time(nullptr));
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);

  loop();

  IMG_Quit();
  SDL_Quit();

  return 0;
}
