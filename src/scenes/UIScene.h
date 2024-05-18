#ifndef _MAIN_SCENE_
#define _MAIN_SCENE_

#include "Scene.h"
#include "Timer.h"

class UIScene : public Scene
{
private:
  float fps;
  bool viewFps;
  Timer fpsViewClock;

  int credits, score, hiScore;

public:
  UIScene();
  ~UIScene();

  void processEvent(const SDL_Event &event) override;
  void update(float delta) override;
  void render(SDL_Renderer *renderer) override;
};

#endif /* _MAIN_SCENE_ */
