#pragma once

#include "../fsms.h"
#include <SDL.h>
#include <SDL_ttf.h>


class App
{
public:
  static void start();

  // globals

  Scene *scene;
  int score, hiScore, coins;
  Uint32 dt;

  SDL_Event event;

  // utils

  // key functions

  void renderText(int x, int y, const char *text,
                  SDL_Color color = {255, 255, 255});
  void renderClip(int x, int y, const SDL_Rect &clip);

private:
  // internals

  SDL_Window *window;
  SDL_Renderer *renderer;

  // assets

  SDL_Texture *atlas;
  TTF_Font *font;

  // not alowed to directly instantiate or destroy

  App();
  ~App();

  void loadAssets();
  void freeAssets();

  void mainLoop();

  void processEvent();
  void update();
  void render();
};

// singleton
extern App *app;
