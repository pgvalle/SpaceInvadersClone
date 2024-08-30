#include <core/Engine.h>

#include <ctime>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_FontCache.h>

int main(int argc, char **argv)
{
  srand(time(nullptr));
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);

  Engine::start();

  IMG_Quit();
  SDL_Quit();

  return 0;
}
