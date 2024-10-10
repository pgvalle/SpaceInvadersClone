#pragma once

#include "Scene.h"
#include "entts/Explosion.h"
#include "entts/UFO.h"

struct PlayScene : Scene
{
  UFO ufo;
  std::vector<Explosion *> explosions;

  PlayScene();
  ~PlayScene() override;

  void processEvent(const SDL_Event &event) override;
  void tick() override;
  void draw() const override;
};
