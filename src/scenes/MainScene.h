#pragma once

#include "Scene.h"

class MainScene : public Scene {
private:

public:
  MainScene();
  ~MainScene();

  //bool processEvent(const SDL_Event &event) override;
  void update(Uint32 delta) override;
  void render(SDL_Renderer *renderer) override;
};
