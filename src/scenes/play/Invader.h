#pragma once

#include <SDL.h>


struct Invader
{
  int x, y;
  SDL_Rect clip;

  Invader(int col, int row);

  void move(int xOff, int yOff);
  void render();
};
