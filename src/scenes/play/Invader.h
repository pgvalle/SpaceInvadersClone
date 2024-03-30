#pragma once

#include <SDL.h>


struct Invader
{
  int type, frame;
  int x, y;

  Invader(int col, int row);

  void move(int xOff, int yOff);
  void render();
};
