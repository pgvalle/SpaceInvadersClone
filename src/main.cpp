#include "globals.h"
#include "entities/entities.h"

static inline
float getTime() {
  return 1e-3 * SDL_GetTicks();
}

void init();
void destroy();

int loadHighScore();

void loop();
void processEvent(const SDL_Event &event);
void update(float dt);
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

  running = true;
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
  const float targetDt = 1.0 / FPS;
  float dt = 0, accum = 0;

  while (running)
  {
    const float before = getTime();

    while (accum < targetDt)
    {
      SDL_Event event;

      if (SDL_WaitEventTimeout(&event, targetDt - accum))
        processEvent(event);

      const float totalTimeWasted = getTime() - before;
      accum = totalTimeWasted;
    }

    update(dt);
    render();
    
    dt = getTime() - before;
    accum = targetDt - dt;
  }
}

void processEvent(const SDL_Event &event)
{
  if (event.type == SDL_QUIT) {
    running = false;
    return;
  }

  
}

void update(float dt)
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
