#include "core.h"

#include <time.h>


void loop();

void process_event();
void update();
void render();


int main()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  srand(time(NULL));

  ctx::init();

  loop();

  ctx::destroy();

  IMG_Quit();
  TTF_Quit();
  SDL_Quit();

  return 0;
}


void loop()
{
  // time measurement
  int64_t before = 0, before_event = 0, timeout = FRAMERATE;

  while (ctx::screen != Screen::EXIT)
  {
    ctx::event.type = 0;
    if (SDL_WaitEventTimeout(&ctx::event, timeout))
    {
      process_event();
      if (ctx::event.type == SDL_QUIT) {
        ctx::screen = Screen::EXIT;
      }

      const int64_t event_dt = SDL_GetTicks() - before_event;
      before_event += event_dt;
      // wait less next time
      timeout -= event_dt;
    }
    else
    {
      update();
      render();

      ctx::dt = SDL_GetTicks() - before;
      before += ctx::dt;
      // reset event timing
      before_event = before;
      timeout = FRAMERATE;
    }
  }
}


void process_event()
{

}

void update()
{

}

void render()
{

}
