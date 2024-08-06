#ifndef SCENE
#define SCENE

#include <SDL2/SDL.h>

class Engine;

class Scene {
public:
  virtual ~Scene() {}

  virtual void process_event(Engine *eng, const SDL_Event &evt) = 0;
  virtual void update(Engine *eng) = 0;
  virtual void render(const Engine *eng) = 0;
};

#endif  // SCENE
