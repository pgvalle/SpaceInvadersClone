#include "core/Engine.hpp"

int main(int argc, char **argv)
{
  Engine::init();
  
  Engine *eng = new Engine;
  delete eng;

  Engine::terminate();

  return 0;
}
