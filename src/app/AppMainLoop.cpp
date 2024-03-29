#include "../core.h"
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
}

void App::render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // Jesus easteregg
  renderText(19 * TILE, TILE, "JESUS LU");
  renderText(19 * TILE, 3 * TILE, "   S2");

  static char valueFmt[7];

  // score
  renderText(TILE, TILE, "SCORE<1>");
  sprintf(valueFmt, "%06d", score);
  renderText(2 * TILE, 3 * TILE, valueFmt);

  // high-score
  renderText(10 * TILE, TILE, "HI-SCORE");
  sprintf(valueFmt, "%06d", hiScore);
  renderText(11 * TILE, 3 * TILE, valueFmt);

  // credits
  renderText(17 * TILE, HEIGHT - 2 * TILE, "CREDIT");
  sprintf(valueFmt, "%02d", coins);
  renderText(WIDTH - 4 * TILE, HEIGHT - 2 * TILE, valueFmt);

  scene->render();

  SDL_RenderPresent(renderer);
}
