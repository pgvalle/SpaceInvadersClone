#include "core.h"

namespace ctx
{
  bool wait_event_timeout(int timeout)
  {
    if (timeout < 0)
    {
      timeout = 0;
    }

    event.type = 0;
    return SDL_WaitEventTimeout(&event, timeout);
  }

  void process_event()
  {
    if (event.type == SDL_QUIT)
    {
      screen = Screen::EXIT;
    }
  }

  void update()
  {
  }

  void render()
  {
  }


  void main_loop()
  {
    // time measurement
    int64_t before = 0, before_event = 0, timeout = FRAMERATE;

    while (screen != Screen::EXIT)
    {
      if (wait_event_timeout(timeout))
      {
        process_event();

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
}
