#include "Engine.h"
#include <SDL.h>

Engine::Engine()
{
  shouldStop = false;
}

Engine::~Engine()
{
}

void Engine::loop()
{
  const Uint32 msPerTick = 1000 / TICKRATE;
  Uint32 msPerFrame = 1, msAccum = 0;

  while (!shouldStop)
  {
    const Uint32 msStart = SDL_GetTicks();

    while (msAccum >= msPerTick)
    {
      tick(1e-3 * msPerTick);
      msAccum -= msPerTick;
    }

    update(1e-3 * msPerFrame);
    render(1e-3 * msPerFrame);

    msPerFrame = SDL_GetTicks() - msStart;
    msAccum += msPerFrame;
  }
}

static Engine *instance = nullptr;

Engine *instantiateEngine();

void Engine::start()
{
  if (instance)
    return;

  instance = instantiateEngine();
  instance->loop();
  delete instance;
  instance = nullptr;
}

Engine *Engine::getInstance()
{
  return instance;
}