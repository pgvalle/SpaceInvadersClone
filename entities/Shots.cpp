#include "Shots.h"
#include "App.h"

void Shot::update(float delta)
{
  y += delta * yVel;
}

void Shot::render() const
{
  App::renderTile(x, y, 9);
}
