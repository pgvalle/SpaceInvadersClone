#include "MyEngine.h"

#include <cassert>

Engine *instantiateEngine()
{
  return new MyEngine();
}

MyEngine::MyEngine() : Engine()
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
}

MyEngine::~MyEngine()
{
}

void MyEngine::tick(float dt)
{
}

void MyEngine::update(float dt)
{
  SDL_Event event;
  SDL_PollEvent(&event);

  if (event.type == SDL_QUIT)
    shouldStop = true;
}

void MyEngine::render(float dt)
{
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  // here goes everything to render

  SDL_RenderPresent(ren);
}
