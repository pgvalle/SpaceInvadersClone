#pragma once

#include <SDL.h>

struct Invader {
  int x, y;
  SDL_Rect clip;

  Invader(int row, int col);

  void move(int xoff, int yoff);
  void render();
};
