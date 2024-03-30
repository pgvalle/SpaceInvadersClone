#pragma once

#include "Scene.h"
#include "scenes/play/Horde.h"


class PlayScene : public Scene
{
private:
  Horde horde;

public:
  PlayScene();
  ~PlayScene();

  void processEvent();
  void update();
  void render();
};
