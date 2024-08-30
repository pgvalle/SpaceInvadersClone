#ifndef CORE_ENGINE
#define CORE_ENGINE

#define TICKRATE 20

class Engine
{
public:
  static void start();
  static Engine *getInstance();

  bool shouldStop;

protected:
  Engine();
  virtual ~Engine();

  void loop();

  virtual void tick(float dt) = 0;
  virtual void update(float dt) = 0;
  virtual void render(float dt) = 0;
};

#endif // CORE_ENGINE