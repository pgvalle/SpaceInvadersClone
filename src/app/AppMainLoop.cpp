#include "../core.h"
#include <stdio.h>


bool waitEventTimeout(int timeout)
{
  // only positive time makes sense
  if (timeout < 0) timeout = 0;

  app->event.type = 0;
  return SDL_WaitEventTimeout(&app->event, timeout);
}


void App::mainLoop()
{
  // time measurement
  int64_t before = 0, before_event = 0, timeout = FRAMERATE;

  while (scene)
  {
    if (waitEventTimeout(timeout))
    {
      processEvent();

      const int64_t event_dt = SDL_GetTicks() - before_event;
      before_event += event_dt;
      // wait less next time
      timeout -= event_dt;
    }
    else
    {
      update();
      render();

      dt = SDL_GetTicks() - before;
      before += dt;
      // reset event timing
      before_event = before;
      timeout = FRAMERATE;
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
}

void App::update()
{
  scene->update();
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


  SDL_RenderPresent(renderer);
}
