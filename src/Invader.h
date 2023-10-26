#pragma once

#include "utils.h"

struct Invader {
  int x, y;
  SDL_Rect clip;

  Invader(int row, int col) {
    x = 64 + 16 * row;
    y = 26 + 16 * col;
    // invaders have different shapes depending on their starting row
    clip = (SDL_Rect){ 12, 16, 12, 8 };
    if (row > 2) clip.y = 32;
    if (row > 0) clip.y = 24;
  }

  void move(int xoff, int yoff) {
    x += xoff;
    y += yoff;
    // each movement should change animation frame
    clip.x = (clip.x == 0 ? 12 : 0);
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    render_clip(x, y, clip, renderer, atlas);
  }
};
