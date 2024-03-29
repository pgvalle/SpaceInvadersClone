#pragma once

#include <SDL.h>
#include <SDL_image.h>

#define FPS 60
#define FRAMERATE (1000 / FPS)

#define TILE 8
// These are the dimensions of the canvas. NOT of the window.
#define WIDTH  (28 * TILE) // 240
#define HEIGHT (32 * TILE) // 224


namespace App
{
  // context

  enum Screen
  {
    MENU,
    PLAY,
    PAUSE,
    OVER,
    EXIT
  } screen;
  int score, hi_score, credits;

  SDL_Event event;
  Uint32 delta;

  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Texture *atlas;

  // utils

  int world_width();
  int world_height();

  bool is_key_pressed(int key);

  void render_clip(int x, int y, const SDL_Rect &clip);
  void render_text(int x, int y, const char *text);
}




