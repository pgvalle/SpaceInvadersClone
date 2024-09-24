#include "glob.h"
#include "common.h"

void tick(float dt)
{
}

void update(float dt, const SDL_Event &event)
{
  if (event->type == SDL_QUIT)
    g->shouldStop = true;
}

void render()
{
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  // here goes everything to render

  SDL_RenderPresent(ren);
}

void loop()
{
  const Uint32 msPerTick = 1000 / TICKRATE;
  Uint32 msPerFrame = 1, msAccum = 0;

  while (!g->shouldStop)
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
    render();

    msPerFrame = SDL_GetTicks() - msStart;
    msAccum += msPerFrame;
  }
}


