#include "../core.h"

#include <string.h>


static const char* TEXTS[] = {
  "PLAY",
  "SPACE INVADERS",
  "=? MYSTERY",
  "=30 POINTS",
  "=20 POINTS",
  "=10 POINTS"
};


MenuScene::MenuScene()
{
  clock.reset(500);
  charI = 0;
}

MenuScene::~MenuScene()
{
  
}


void MenuScene::processEvent()
{

}

void MenuScene::update()
{
  switch (state)
  {
  case WAITING:
    clock.update();
    if (clock.hasTimedOut())
    {
      state = DISPLAYING_PLAY;
      clock.reset(100);
      charI = 0;
    }
    break;

  case IDLE:
    break;

  default:
    clock.update();
    if (clock.hasTimedOut())
    {
      if (++charI >= strlen(TEXTS[state]))
      {
        state++;
        charI = 0;
      }
      
      clock.reset(100);
    }
  }
}

void MenuScene::render()
{
  switch (state)
  {
  case WAITING:
    break;

  case IDLE:
  
    break;

  default:
    for (int i = 0; i < (int)state; i++)
    {

    }

    static char buffer[17];
    strncpy(buffer, TEXTS[state], charI + 1);
    app->renderText(0, 0, buffer);
  
  case DISPLAYING_PLAY:
    strncpy(buffer, TEXTS[0], charI + 1);
    app->renderText(0, 0, buffer);
    break;

  case DISPLAYING_SPACE_INVADERS:
    app->renderText(0, 0, TEXTS[0]);

    strncpy(buffer, TEXTS[1], charI + 1);
    app->renderText(0, 16, buffer);
    break;

  case DISPLAYING_SCORE_INFO:
    app->renderText(0, 0, TEXTS[0]);
    app->renderText(0, 16, TEXTS[1]);

    for (int i = 2; i < 6; i++) {
      strncpy(buffer, TEXTS[i], charI + 1);
      app->renderText(0, 24 + (i - 2) * TILE, buffer);
    }
    
    break;
  }
}
