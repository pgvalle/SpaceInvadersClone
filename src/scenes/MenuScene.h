#pragma once

#include "Scene.h"
#include "../utils/Timer.h"


class MenuScene : public Scene
{
private:
  Timer clock; // dummy clock

public:
  MenuScene();
  ~MenuScene();

  void processEvent(const SDL_Event &event);
  void update(Uint32 delta);
  void render(SDL_Renderer *renderer);
};
