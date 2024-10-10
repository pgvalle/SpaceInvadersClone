#pragma once

#include "common.h"
#include "scenes/Scene.h"

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

#define TICKRATE 30

struct SIC
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *atlas;
  FC_Font *font;
  Scene *currentScene, *nextScene;
  bool shouldStop;
  int score, highScore;

  SIC();
  ~SIC();

  static void start();
  void loop();
};

extern SIC *sic;

