#pragma once

#include "Scene.h"
#include "../utils/Timer.h"


class MenuScene : public Scene
{
private:
  Timer clock; // dummy clock

public:
  MenuScene();
  ~MenuScene();

  void processEvent();
  void update();
  void render();
};
