#ifndef GLOBALS
#define GLOBALS

#include <common.h>

DECL(SDL_Window *, win);
DECL(SDL_Renderer *, ren);
DECL(SDL_Texture *, atlas);
DECL(FC_Font *, font);
DECL(int, score, highScore);
DECL(bool, running);

static inline
void addToScore(int val) {
  score += val;
  if (score > highScore) highScore = score;
}

#endif  // GLOBALS
