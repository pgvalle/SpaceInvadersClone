#pragma once

#include <SDL.h>

class Scene {
protected:
  Scene *parent = nullptr;

public:
  virtual ~Scene() {}

  // returns true if event was processed. 
  virtual void processEvent(const SDL_Event &event) = 0;
  virtual void update(Uint32 delta) = 0;
  virtual void render(SDL_Renderer *renderer) = 0;
};
