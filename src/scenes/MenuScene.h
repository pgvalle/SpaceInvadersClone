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
  int charI, textI; // iterators

public:
  MenuScene();

  void processEvent();
  void update();
  void render();
};
