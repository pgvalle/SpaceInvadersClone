#pragma once

struct Shot
{
  int x, y, yVel;

  void update(float delta);
  virtual void render() const;
};

struct InvaderShot : Shot
{
  int type;


}
