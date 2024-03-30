#pragma once

#include "Scene.h"
#include "./play/Horde.h"
#include "./play/UFO.h"


class PlayScene : public Scene
{
private:
  UFO ufo;
  Horde horde;

public:
  PlayScene();
  ~PlayScene();

  void processEvent();
  void update();
  void render();
};
