#include "UIScene.h"
#include "Events.h"

#include "App.h"

#include <cstdio>

UIScene::UIScene()
{
  fps = 0;
  viewFps = true;
  fpsViewClock.reset(1);

  credits = 0;
  score = 0;
  hiScore = 0;

  FILE *f = fopen("hiscore.txt", "r");
  if (!f) {
    f = fopen("hiscore.txt", "w");
  }
  fscanf(f, "%d", &hiScore);
  fclose(f);
}

UIScene::~UIScene()
{
  FILE *f = fopen("hiscore.txt", "w");
  fprintf(f, "%d", hiScore);
  fclose(f);
}

void UIScene::processEvent(const SDL_Event &event)
{
  switch (event.type)
  {
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
      case SDLK_f:
        viewFps = !viewFps;
        printf("FPS view turned %s\n", viewFps ? "on" : "off");
        break;
      case SDLK_q:
        //NAGE::pushEvent(, null, 0);
        break;
      case SDLK_y:
      {
        Uint16 addition = rand() % 300;
        NAGE::pushEvent(SCORE_UPDATE_EVENT, &addition, sizeof(addition));
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
      score += NAGE::getEventData<Uint8>(event.user, 0);
      // hiScore needs to keep up with score
      if (score > hiScore)
      {
        hiScore = score;
      }
    }
    break;

  default:
    break;
  }
}

void UIScene::update(float delta)
{
  fpsViewClock.update(delta);
  if (fpsViewClock.hasTimedOut())
  {
    fps = 1 / delta;
    fpsViewClock.reset();
  }
}

void UIScene::render(SDL_Renderer *renderer)
{
  // clear screen with black color
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  NAGE::renderText(8, 8, "UR SCORE          HI-SCORE\n\n"
                  " %06d            %06d", score, hiScore); // scores
  // fps
  if (viewFps)
  {
    NAGE::renderText(13 * 8, 16, "%2d", (int)fps);
  }
  // credits
  NAGE::renderText(17 * 8, 256 - 2 * 8, "CREDIT %02d", credits);
}
