#include "app/App.h"
#include "Invader.h"
#include "defines.h"


Invader::Invader(int col, int row) {
  x = 3 * TILE + 2 * TILE * col;
  y = 8 * TILE + 2 * TILE * row;
  // invaders have different shapes depending on their starting row
  clip = { 12, 16, 12, 8 };
  if (row > 2) clip.y = 32;
  else if (row > 0) clip.y = 24;
}

void Invader::move(int xOff, int yOff) {
  x += xOff;
  y += yOff;
  // each movement should change animation frame
  clip.x = (clip.x == 0 ? 12 : 0);
}

void Invader::render() {
  app->renderClip(x, y, clip);
}
