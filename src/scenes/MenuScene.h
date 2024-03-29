#pragma once

#include "Scene.h"
#include "../utils/Timer.h"


class MenuScene : Scene
{
private:
  enum State
  {
    WAITING,
    DISPLAYING_TEXTS
  } state;

  Timer clock;

public:
  MenuScene();

  void processEvent();
  void update();
  void render();
};
