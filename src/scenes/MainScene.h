#pragma once

#include "Scene.h"

class MainScene : public Scene {
public:
  int coins, score, hiScore;

  MainScene();
  ~MainScene();

  void processEvent(const SDL_Event &event) override;
  void update(Uint32 delta) override;
  void render(SDL_Renderer *renderer) override;
};
