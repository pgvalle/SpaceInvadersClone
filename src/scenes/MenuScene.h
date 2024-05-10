#pragma once

#include "Scene.h"
#include "Timer.h"

class MenuScene : public Scene {
private:
  Timer clock; // dummy clock

public:
  MenuScene();
  ~MenuScene();

  void processEvent(const SDL_Event &event) override;
  void update(float delta) override;
  void render(SDL_Renderer *renderer) override;
};
