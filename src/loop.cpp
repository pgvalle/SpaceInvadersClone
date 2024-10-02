#include "glob.h"
#include "common.h"

void tick(float dt)
{
  g->ufo.update(dt);
  g->ufo.onHit({0, 0, 20, 500});
}

void update(float dt, const SDL_Event &event)
{
  if (event.type == SDL_QUIT)
    g->shouldStop = true;
}

void render()
{
  SDL_SetRenderDrawColor(g->ren, 0, 0, 0, 255);
  SDL_RenderClear(g->ren);

  // here goes everything to render
  g->ufo.render();

  SDL_RenderPresent(g->ren);
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
