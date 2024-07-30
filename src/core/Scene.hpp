#ifndef SCENE
#define SCENE

class Engine;

class Scene {
public:
  virtual void process_event(Engine *eng, const SDL_Event &evt);
  virtual void update(Engine *eng);
  virtual void render(const Engine *eng);
};

#endif  // SCENE
