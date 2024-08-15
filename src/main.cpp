#include "globals.h"
#include "entities/entities.h"

void init();
void destroy();

int loadHighScore();

void loop();
void processEvent(const SDL_Event &event);
void update();
void render();

int main(int argc, char **argv)
{
  init();
  loop();
  destroy();

  return 0;
}

void init()
{
  srand(time(nullptr));
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);

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
  highScore = loadHighScore();
}

void destroy()
{
  FC_FreeFont(font);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  IMG_Quit();
  SDL_Quit();
}

void loop()
{
  const Uint32 targetDelta = 1000 / FPS;

  while (true)
  {
    Uint32 delta = 0;

    while (delta < targetDelta)
    {
      const Uint32 before = SDL_GetTicks();
      SDL_Event event;

      if (SDL_WaitEventTimeout(&event, targetDelta - delta))
      {
        if (event.type == SDL_QUIT)
          return;
        processEvent(event);
      }

      const Uint32 timeWasted = SDL_GetTicks() - before;
      delta += timeWasted;
    }

    update();
    render();
  }
}

void processEvent(const SDL_Event &event)
{
}

void update()
{
  ufo.update();
  horde.update();
  updateExplosions();
}

void render()
{
  SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
  SDL_RenderClear(ren);
  ufo.render();
  horde.render();
  renderExplosions();
  SDL_RenderPresent(ren);
}

int loadHighScore()
{
  return 0;
}
