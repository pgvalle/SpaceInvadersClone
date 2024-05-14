#include "MainScene.h"
#include "App.h"

#include <cstdio>

MainScene::MainScene()
{
  fps = 0;
  viewFps = true;
  fpsUpdateClock.reset(1);

  credits = 0;
  score = 0;
  hiScore = 0;
}

MainScene::~MainScene()
{
}

void MainScene::processEvent(const SDL_Event &event)
{
  switch (event.type)
  {
  case SDL_QUIT:
    shouldClose = true;
    break;

  case SDL_KEYDOWN:
  {
    const SDL_Keycode key = event.key.keysym.sym;
    switch (key)
    {
      case SDLK_EQUALS:
        credits += (credits < 99 ? 1 : 0);
        break;
      case SDLK_MINUS:
        credits -= (credits > 0 ? 1 : 0);
        break;
      case SDLK_q:
        viewFps = !viewFps;
        printf("FPS view turned %s\n", viewFps ? "on" : "off");
        break;
      default:
        break;
    }
    break;
  }

  case SDL_USEREVENT:
    if (event.user.code == SCORE_UPDATE_EVENT)
    {
      score += getUint8FromUserEvent(event.user, 0);
      // hiScore needs to keep up with score
      if (score > hiScore)
      {
        hiScore = score;
      }

      freeUserEventData(event);
    }
    break;

  default:
    break;
  }
}

void MainScene::update(float delta)
{
  fpsUpdateClock.update(delta);
  if (fpsUpdateClock.hasTimedOut())
  {
    fps = 1 / delta;
    fpsUpdateClock.reset();
  }
}

void MainScene::render(SDL_Renderer *renderer)
{
  // clear screen with black color
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // rendering globals as part of UI

  renderText(8, 8, "SCORE<1>          HI-SCORE\n\n"
             " %06d            %06d", score, hiScore); // scores
  // fps
  if (viewFps)
  {
    renderText(13 * 8, 16, "%2d", (int)fps);
  }
  // credits
  renderText(17 * 8, 256 - 2 * 8, "CREDIT %02d", credits);
}
