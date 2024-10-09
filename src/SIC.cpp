#include "SIC.h"
#include "common.h"

#include "scenes/PlayScene.h"
Scene *s = NULL;
SIC::SIC()
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

  atlas = IMG_LoadTexture(ren, "res/atlas.png");
  assert(atlas);

  font = FC_CreateFont();
  FC_LoadFont(
    font,
    ren,
    "res/ps2p.ttf",
    TILE,
    FC_MakeColor(255, 255, 255, 255),
    TTF_STYLE_NORMAL);

  score = 0;
  highScore = 0;
  shouldStop = false;
}

SIC::~SIC()
{
}

void SIC::add2Score(int value)
{
  score += value;
  if (score > highScore)
    highScore = score;
}

void SIC::loop()
{
  s = new PlayScene;

  const Uint32 msPerTick = 1000 / TICKRATE;
  Uint32 msPerFrame = 1, msAccum = 0;

  while (!shouldStop)
  {
    const Uint32 msStart = SDL_GetTicks();

    while (msAccum >= msPerTick)
    {
      onTick(1e-3 * msPerTick);
      msAccum -= msPerTick;
    }

    SDL_Event event;
    SDL_PollEvent(&event);
    onUpdate(1e-3 * msPerFrame, event);
    onRender();

    msPerFrame = SDL_GetTicks() - msStart;
    msAccum += msPerFrame;
  }

  delete s;
}

void SIC::onTick(float dt)
{
}

void SIC::onUpdate(float dt, const SDL_Event &event)
{
  if (event.type == SDL_QUIT)
    shouldStop = true;
  s->onUpdate(dt);
}

void SIC::onRender() const
{
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);

  // here goes everything to render
  s->onRender();

  SDL_RenderPresent(ren);
}

SIC *g = NULL;
