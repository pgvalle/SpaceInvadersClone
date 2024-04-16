#define APP_INTERNALS
#include "app/App.h"
#include "defines.h"

#include <stdio.h>


void processEvent();
void update();
void render();

// for time measurement inside mainLoop
static int64_t before = 0, beforeEvent = 0, timeout = FRAMERATE;

bool gotEventWithinTimeout()
{
  // only positive time makes sense
  if (timeout < 0) timeout = 0;

  event.type = 0;
  return SDL_WaitEventTimeout(&event, timeout);
}

void mainLoop()
{
  while (scene)
  {
    if (gotEventWithinTimeout())
    {
      processEvent();
    }
    else
    {
      update();
      render();
    }

    // process scene change, if any
    if (sceneChange)
    {
      delete scene;
      scene = nextScene;
      nextScene = nullptr;
      sceneChange = false;
    }
  }
}

void processEvent()
{
  if (event.type == SDL_QUIT)
  {
    delete scene;
    scene = nullptr;
  }
  else
  {
    scene->processEvent();
  }

  const int64_t eventDt = SDL_GetTicks() - beforeEvent;
  beforeEvent += eventDt;
  // wait less next time
  timeout -= eventDt;
}

void update()
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

#include "utils/Timer.h"

static Timer t1, t2;
static char JesusIs[7] = " LORD"; 
void render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  static char valueFmt[27];

  renderText(TILE, TILE, "SCORE<1> HI-SCORE JESUS IS");

  sprintf(valueFmt, "%06d   %06d", score, hiScore);
  t1.update();
  if (t1.hasTimedOut())
  {
    sprintf(valueFmt, "%06d   %06d   %s", score, hiScore, JesusIs);
    t2.update();
    if (t2.hasTimedOut())
    {
      sprintf(JesusIs, "%s", JesusIs[0] == 'S' ? "SAVIOR" : " LORD");
      t1.reset(1000);
      t2.reset(1000);
    }
  }
  renderText(2 * TILE, 3 * TILE, valueFmt);

  // credits
  sprintf(valueFmt, "CREDIT %02d", coins);
  renderText(17 * TILE, HEIGHT - 2 * TILE, valueFmt);

  scene->render();

  SDL_RenderPresent(renderer);
}
