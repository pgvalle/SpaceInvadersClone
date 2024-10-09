#pragma once

struct Scene
{
  virtual ~Scene() {};

  virtual void onTick(float dt) = 0;
  virtual void onUpdate(float dt) = 0;
  virtual void onRender() const = 0;
};
