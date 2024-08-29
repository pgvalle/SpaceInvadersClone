#include "App.h"

App::App()
{
  shouldClose = false;
}

App::~App()
{
}

void App::loop()
{
  const Uint32 msPerTick = 1000 / TICKRATE;
  Uint32 msPerFrame = 1, msAccum = 0;

  while (!shouldClose)
  {
    const Uint32 msStart = SDL_GetTicks();

    while (msAccum >= msPerTick)
    {
      tick(1e-3 * msPerTick);
      msAccum -= msPerTick;
    }

    SDL_Event event;
    SDL_PollEvent(&event);
    update(1e-3 * msPerFrame, event);
    render(1e-3 * msPerFrame);

    msPerFrame = SDL_GetTicks() - msStart;
    msAccum += msPerFrame;
  }
}
