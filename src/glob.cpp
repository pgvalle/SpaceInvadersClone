#include "glob.h"
#include "common.h"

void updateScore(int value)
{
  g->score += value;
  if (g->score > g->highScore)
    g->highScore = g->score;
}

void init()
{
  srand(time(nullptr));
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  
  g->win = SDL_CreateWindow(
    "Space Invaders Clone",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    2 * WIDTH,
    2 * HEIGHT,
    SDL_WINDOW_RESIZABLE);
  assert(g->win);

  g->ren = SDL_CreateRenderer(g->win, -1, SDL_RENDERER_ACCELERATED);
  assert(g->ren);
  SDL_RenderSetLogicalSize(g->ren, WIDTH, HEIGHT);

  g->atlas = IMG_LoadTexture(g->ren, "../res/atlas.png");
  assert(g->atlas);

  g->font = FC_CreateFont();
  FC_LoadFont(
    g->font,
    g->ren,
    "../res/ps2p.ttf",
    TILE,
    FC_MakeColor(255, 255, 255, 255),
    TTF_STYLE_NORMAL);

  g->score = 0;
  g->highScore = 0;
  g->shouldStop = false;
}

void terminate()
{
  IMG_Quit();
  SDL_Quit();
}

struct Global *g;
