#pragma once

#include "common.h"
#include "Scene.h"
#include "entts/Bunker.h"
#include "entts/UFO.h"
#include "entts/Horde.h"
#include "entts/Explosion.h"

struct PlayScene : Scene
{
  UFO ufo;
  Horde horde;
  Bunker bunkers[4];
  std::vector<Explosion *> explosions;

  PlayScene();
  ~PlayScene() override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;
};