#pragma once

#include <SDL.h>

class Scene {
public:
  virtual ~Scene() {}

  // returns true if event was processed. 
  virtual bool processEvent(const SDL_Event &event);
  virtual void update(Uint32 delta) = 0;
  virtual void render(SDL_Renderer *renderer) = 0;
};
