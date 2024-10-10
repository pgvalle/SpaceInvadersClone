#include "SIC.h"
#include "common.h"
#include "scenes/PlayScene.h"

SIC::SIC()
{
  window = SDL_CreateWindow(
      "Space Invaders Clone",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      2 * WIDTH,
      2 * HEIGHT,
      SDL_WINDOW_RESIZABLE);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

  atlas = IMG_LoadTexture(renderer, "res/atlas.png");
  assert(atlas);

  font = FC_CreateFont();
  assert(FC_LoadFont(
      font,
      renderer,
      "res/ps2p.ttf",
      TILE,
      {255, 255, 255, 255},
      TTF_STYLE_NORMAL));

  currentScene = new PlayScene;
  nextScene = NULL;

  shouldStop = false;
  score = 0;
  highScore = 0; // TODO: implement loading high score from file
}

SIC::~SIC()
{
  delete currentScene;
  FC_ClearFont(font);
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void SIC::start()
{
  srand(time(NULL)); // so that we have random numbers

  // initialize libraries
  SDL_Init(SDL_INIT_EVERYTHING);
  TTF_Init();
  IMG_Init(IMG_INIT_PNG);

  sic = new SIC;
  sic->loop();
  delete sic;
  sic = NULL;

  // terminate libraries
  IMG_Quit();
  TTF_Quit();
  SDL_Quit();
}

void SIC::loop()
{
  const Uint32 msPerTick = 1000 / TICKRATE;
  Uint32 msPerFrame = 1, msAccum = 0;

  while (!shouldStop)
  {
    const Uint32 msStart = SDL_GetTicks();

    // fixed time update (tick)
    while (msAccum >= msPerTick)
    {
      currentScene->tick();
      msAccum -= msPerTick;
    }

    // event processing
    SDL_Event event;
    if (SDL_PollEvent(&event))
      currentScene->processEvent(event);

    // rendering
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    currentScene->draw();
    SDL_RenderPresent(renderer);

    // update highScore
    if (score > highScore)
      highScore = score;

    // update scene
    if (nextScene)
    {
      delete currentScene;
      currentScene = nextScene;
      nextScene = NULL;
    }

    // End of loop. Calculate accumulated time.
    msPerFrame = SDL_GetTicks() - msStart;
    msAccum += msPerFrame;
  }
}

SIC *sic = NULL;
