#include <common.h>
#include <scenes/game/Game.h>

int main(int argc, char **argv)
{
  Engine::init();
  
  eng = new Engine(new Game());
  delete eng;
  eng = nullptr;

  Engine::terminate();

  return 0;
}
