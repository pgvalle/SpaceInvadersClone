#pragma once

#include "utils.h"

class Invader {
private:
  int m_x, m_y;
  SDL_Rect m_clip;

public:
  Invader(int row, int col) {
    m_x = 64 + 16 * row;
    m_y = 26 + 16 * col;
    // invaders have different shapes depending on their starting row
    m_clip = (SDL_Rect){ 12, 16, 12,  8 };
    if (row > 2) m_clip.y = 32;
    if (row > 0) m_clip.y = 24;
  }

  void move(int xoff, int yoff) {
    m_x += xoff;
    m_y += yoff;
    // each movement should change animation frame
    m_clip.x = (m_clip.x == 0 ? 12 : 0);
  }

  void render(SDL_Renderer* renderer, SDL_Texture* atlas) {
    render_clip(m_x, m_y, m_clip, renderer, atlas);
  }
};
