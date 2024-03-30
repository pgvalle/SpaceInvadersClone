#pragma once

#include "Scene.h"
#include "../utils/Timer.h"


class MenuScene : public Scene
{
private:
  enum State
  {
    WAITING = -1,
    DISPLAYING_PLAY = 0,
    DISPLAYING_SPACE_INVADERS,
    DISPLAYING_SCORE_INFO,
    IDLE,
  } state;

  Timer clock;
  int charI; // iterator in text

public:
  MenuScene();
  ~MenuScene();

  void processEvent();
  void update();
  void render();
};
