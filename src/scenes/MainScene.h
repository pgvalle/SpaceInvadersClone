#pragma once

#include "Scene.h"

class MainScene : public Scene {
private:
  int coins, score, hiScore;

public:
  MainScene();
  ~MainScene();

  void processEvent(const SDL_Event &event) override;
  void update(Uint32 delta) override;
  void render(SDL_Renderer *renderer) override;
};
