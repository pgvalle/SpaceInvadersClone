#include "app/App.h"
#include "defines.h"

#include <stdio.h>


// for time measurement inside mainLoop
static int64_t before = 0, beforeEvent = 0, timeout = FRAMERATE;

bool waitEventWithTimeout()
{
  // only positive time makes sense
  if (timeout < 0) timeout = 0;

  app->event.type = 0;
  return SDL_WaitEventTimeout(&app->event, timeout);
}

void App::mainLoop()
{
  while (scene)
  {
    const bool noEvent = !waitEventWithTimeout();
    if (noEvent)
    {
      render();
      update();
    }
    else processEvent();

    // process scene change, if any
    if (nextScene)
    {
      delete scene;
      scene = nextScene;
      nextScene = nullptr;
    }
  }
}

void App::processEvent()
{
  if (event.type == SDL_QUIT)
  {
    delete scene;
    scene = nullptr;
  }
  else scene->processEvent();

  const int64_t eventDt = SDL_GetTicks() - beforeEvent;
  beforeEvent += eventDt;
  // wait less next time
  timeout -= eventDt;
}

void App::update()
{
  scene->update();

  dt = SDL_GetTicks() - before;
  before += dt;
  // reset event timing
  beforeEvent = before;
  timeout = FRAMERATE;

  //printf("\r\x1b[2KFPS: %.2f", 1000.0f / dt);
  //fflush(stdout);
}

void App::render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  static char valueFmt[27];

  renderText(TILE, TILE, "SCORE<1> HI-SCORE JESUS LU");

  sprintf(valueFmt, "%06d   %06d   S2S2S2", score, hiScore);
  renderText(2 * TILE, 3 * TILE, valueFmt);

  // credits
  sprintf(valueFmt, "CREDIT %02d", coins);
  renderText(17 * TILE, HEIGHT - 2 * TILE, valueFmt);

  scene->render();

  SDL_RenderPresent(renderer);
}
