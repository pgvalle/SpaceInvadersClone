#pragma once

#include "common.h"
#include "Scene.h"

struct Entity;
struct Explosion;

struct PlayScene : Scene
{
  std::vector<Entity *> entities;
  std::vector<Explosion *> explosions;

  PlayScene();
  ~PlayScene() override;

  void onTick(float dt) override;
  void onUpdate(float dt) override;
  void onRender() const override;
};