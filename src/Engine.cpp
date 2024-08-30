#include "Engine.h"
#include <SDL.h>

static Engine *instance = nullptr;

void Engine::start()
{
  if (instance)
    return;

  instance = new Engine();
  instance->loop();
  delete instance;
  instance = nullptr;
}

Engine *Engine::getInstance()
{
  return instance;
}

Engine::Engine()
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

Engine::~Engine()
{
}

void Engine::loop()
{
  const Uint32 msPerTick = 1000 / TICKRATE;
  Uint32 msPerFrame = 1, msAccum = 0;

  while (!shouldStop)
  {
    const Uint32 msStart = SDL_GetTicks();

    while (msAccum >= msPerTick)
    {
      tick(1e-3 * msPerTick);
      msAccum -= msPerTick;
    }

    update(1e-3 * msPerFrame);
    render(1e-3 * msPerFrame);

    msPerFrame = SDL_GetTicks() - msStart;
    msAccum += msPerFrame;
  }
}

void Engine::tick(float dt)
{
}

void Engine::update(float dt)
{
  SDL_Event event;
  SDL_PollEvent(&event);

  if (event.type == SDL_QUIT)
    shouldStop = true;
}

void Engine::render(float dt)
{
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  // here goes everything to render

  SDL_RenderPresent(ren);
}