#pragma once

#include "common.h"
#include "Scene.h"
#include "entts/Bunker.h"
#include "entts/Cannon.h"
#include "entts/Explosion.h"
#include "entts/Horde.h"
#include "entts/UFO.h"
#include "entts/Shot.h"

struct PlayScene : Scene
{
  Cannon cannon;
  int lives;
  Timer shotCooldown;

  UFO ufo;
  Horde horde;
  Bunker bunkers[4];
  std::vector<Shot *> shots;
  std::vector<Explosion *> explosions;

  PlayScene();
  ~PlayScene() override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;
};
