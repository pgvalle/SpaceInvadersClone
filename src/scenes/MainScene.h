#pragma once

#include "Scene.h"
#include "Timer.h"

#define SCORE_UPDATE_EVENT 1

class MainScene : public Scene
{
private:
  float fps;
  bool viewFps;
  Timer fpsUpdateClock;

  int credits, score, hiScore;

public:
  MainScene();
  ~MainScene();

  void processEvent(const SDL_Event &event) override;
  void update(float delta) override;
  void render(SDL_Renderer *renderer) override;
};
