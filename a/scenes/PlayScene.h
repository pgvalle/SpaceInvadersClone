#pragma once

#include "Scene.h"
#include "entts/Bunker.h"
#include "entts/Cannon.h"
#include "entts/Explosion.h"
#include "entts/Shot.h"
#include "entts/UFO.h"

struct PlayScene : Scene
{
  Cannon cannon;

  Bunker bunkers[4];
  UFO ufo;
  std::vector<Explosion *> explosions;
  std::vector<Shot *> shots;

  PlayScene();
  ~PlayScene() override;

  void processEvent(const SDL_Event &event) override;
  void tick() override;
  void draw() const override;
};
