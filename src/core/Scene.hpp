#ifndef SCENE
#define SCENE

class Engine;

class Scene {
public:
  virtual void process_events(Engine &eng);
  virtual void update(Engine &eng);
  virtual void render(const Engine &eng);
};

#endif  // SCENE
