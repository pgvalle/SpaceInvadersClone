#include "MainScene.h"
#include "App.h"

#include <cstdio>

MainScene::MainScene()
{
  fps = 0;
  viewFps = true;
  fpsViewClock.reset(1);

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
      case SDLK_y:
      {
        Uint16 addition = rand() % 300;
        pushUserEvent(SCORE_UPDATE_EVENT, &addition, sizeof(addition));
        break;
      }
      default:
        break;
    }
    break;
  }

  case SDL_USEREVENT:
    if (event.user.code == SCORE_UPDATE_EVENT)
    {
      score += getUint16FromUserEvent(event.user, 0);
      // hiScore needs to keep up with score
      if (score > hiScore)
      {
        hiScore = score;
      }

      freeUserEventData(event.user);
    }
    break;

  default:
    break;
  }
}

void MainScene::update(float delta)
{
  fpsViewClock.update(delta);
  if (fpsViewClock.hasTimedOut())
  {
    fps = 1 / delta;
    fpsViewClock.reset();
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
