#include "glob.h"
#include "common.h"

Glob::Glob()
{
  win = SDL_CreateWindow(
    "Space Invaders Clone",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    2 * WIDTH,
    2 * HEIGHT,
    SDL_WINDOW_RESIZABLE);
  assert(win);

  ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
  assert(ren);
  SDL_RenderSetLogicalSize(ren, WIDTH, HEIGHT);

  atlas = IMG_LoadTexture(ren, "../res/atlas.png");
  assert(atlas);

  font = FC_CreateFont();
  FC_LoadFont(
    font,
    ren,
    "../res/ps2p.ttf",
    TILE,
    FC_MakeColor(255, 255, 255, 255),
    TTF_STYLE_NORMAL);

  score = 0;
  highScore = 0;
  shouldStop = false;
}

Glob::~Glob()
{

}

void Glob::add2Score(int value)
{
  score += value;
  if (score > highScore)
    highScore = score;
}

struct Global *g;
