#include "app/App.h"
#include "Invader.h"


Invader::Invader(int row, int col) {
  x = 64 + 16 * row;
  y = 26 + 16 * col;
  // invaders have different shapes depending on their starting row
  clip = { 12, 16, 12, 8 };
  if (row > 2) clip.y = 32;
  if (row > 0) clip.y = 24;
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
