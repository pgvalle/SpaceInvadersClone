#pragma once

#include "fsms.h"
#include <SDL.h>
#include <SDL_ttf.h>


// with this i don't need to change both header and impl file
#ifdef CTX_IMPL
#define extern
#endif

namespace ctx
{
  // state

  extern enum Screen screen;
  extern int score, hi_score, credits;
  extern Uint32 dt;

  // internals

  extern SDL_Event event;
  extern SDL_Window *window;
  extern SDL_Renderer *renderer;

  // assets

  extern SDL_Texture *atlas;
  extern TTF_Font *font;


  void init();
  void destroy();

  void main_loop();
}

#ifdef CTX_IMPL
#undef extern
#endif

/*
  void increase_score();

  void render_clip(int x, int y, const SDL_Rect &clip);
  void render_text(int x, int y, const char *text);
*/
