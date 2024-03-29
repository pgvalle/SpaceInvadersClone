#pragma once


class Scene
{
public:
  virtual ~Scene() {}

  virtual void processEvent() = 0;
  virtual void update() = 0;
  virtual void render() = 0;
};
