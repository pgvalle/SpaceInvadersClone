#include "../core.h"


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
  
}
