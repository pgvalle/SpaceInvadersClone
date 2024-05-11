#pragma once

#include "Scene.h"
#include "Timer.h"

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

  void addToScore(int value);

  void processEvent(const SDL_Event &event) override;
  void update(float delta) override;
  void render(SDL_Renderer *renderer) override;
};
