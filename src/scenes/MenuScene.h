#pragma once

#include "Scene.h"
#include "../utils/Timer.h"

class MenuScene : public Scene {
private:
  Timer clock; // dummy clock

public:
  MenuScene();
  ~MenuScene();

  //bool processEvent(const SDL_Event &event) override;
  void update(Uint32 delta) override;
  void render(SDL_Renderer *renderer) override;
};
