#ifndef APP_H
#define APP_H

#include <common.h>

#define TICKRATE 20

class App
{
protected:
  bool shouldClose;

  App();
  virtual ~App();

  void loop();

public:
  virtual void tick(float dt) = 0;
  virtual void update(float dt, const SDL_Event &event) = 0;
  virtual void render(float dt) = 0;
};

#endif // APP_H