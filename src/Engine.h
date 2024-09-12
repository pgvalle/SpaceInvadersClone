#ifndef CORE_ENGINE
#define CORE_ENGINE

#include <common.h>

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

#define TICKRATE 20

class Engine
{
private:
  int score, highScore;

public:
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *atlas;
  FC_Font *font;
  bool shouldStop;

  static void start();
  static Engine *getInstance();

private:
  Engine();
  ~Engine();

  void loop();

  void tick(float dt);
  void update(float dt);
  void render(float dt);

  void updateScore(int value);
};

#endif // CORE_ENGINE