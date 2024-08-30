#ifndef MY_ENGINE
#define MY_ENGINE

#include <core/Engine.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_FontCache.h>

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

class MyEngine : public Engine
{
public:
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, highScore;

  MyEngine();
  ~MyEngine() override;

  void tick(float dt) override;
  void update(float dt) override;
  void render(float dt) override;

  void addToScore(int val) {
    score += val;
    if (score > highScore)
      highScore = score;
  }
};

#endif // MY_ENGINE