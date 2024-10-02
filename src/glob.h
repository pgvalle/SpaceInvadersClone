#ifndef GLOB_H
#define GLOB_H

#define TILE 8
#define WIDTH (28 * TILE)
#define HEIGHT (32 * TILE)

#define TICKRATE 20

#include "entts/UFO.h"

struct Glob
{
  SDL_Window *win;
  SDL_Renderer *ren;
  SDL_Texture *atlas;
  FC_Font *font;
  int score, highScore;
  bool shouldStop;
  UFO ufo;

  void add2Score(int val);
};

extern Glob *g;

#endif // GLOB_H
