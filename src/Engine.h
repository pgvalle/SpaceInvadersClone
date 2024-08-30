#ifndef CORE_ENGINE
#define CORE_ENGINE

#include <common.h>
#include <memory>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_FontCache.h>

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

#define TICKRATE 20

class Engine
{
public:
  static void start();
  static Engine *getInstance();

  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, highScore;
  bool shouldStop;

protected:
  Engine();
  virtual ~Engine();

  void loop();

  void tick(float dt);
  void update(float dt);
  void render(float dt);
};

#endif // CORE_ENGINE