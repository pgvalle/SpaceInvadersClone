#pragma once

#include "Scene.h"

struct PlayScene : Scene
{
  PlayScene();
  ~PlayScene() override;

  void processEvent(const SDL_Event &event) override;
  void tick(float dt) override;
  void draw() const override;
};
