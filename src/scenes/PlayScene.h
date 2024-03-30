#pragma once

#include "Scene.h"


class PlayScene : public Scene
{
private:

public:
  PlayScene();
  ~PlayScene();

  void processEvent();
  void update();
  void render();
};
