#define APP_INTERNALS
// no need to redeclare app fields here!
#define EXT

#include "app/App.h"
#include "defines.h"

#include "scenes/Scene.h"
#include "scenes/MenuScene.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

void loadAssets() {
  window = SDL_CreateWindow(
      "Space Invaders Clone",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      2 * WIDTH,
      2 * HEIGHT,
      SDL_WINDOW_RESIZABLE);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // resolution independent rendering

  font = TTF_OpenFont(ASSETS_DIR "ps2p.ttf", 8);

  // image assets
  {
    SDL_Surface *surface = nullptr;
    
    surface = IMG_Load(ASSETS_DIR "atlas.png");
    atlas = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = SDL_LoadBMP(ASSETS_DIR "icon.bmp");
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);
  }

  score = 0;
  hiScore = 0; // load from file
  coins = 0; // load from file

  scene = new MenuScene();
  nextScene = nullptr;
  sceneChange = false;

  // load audio assets
}

void freeAssets() {
  // free audio assets

  SDL_DestroyTexture(atlas);

  TTF_CloseFont(font);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void mainLoop() {
  int64_t before = 0, beforeEvent = 0, timeout = FRAMERATE;

  while (scene) {
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, timeout)) {
      scene->processEvent(event);

      const int64_t eventDt = SDL_GetTicks() - beforeEvent;
      beforeEvent = SDL_GetTicks();
      // wait less next time
      timeout -= eventDt;
    }
    else {
      const int64_t dt = SDL_GetTicks() - before;
      before = SDL_GetTicks();
      // reset event timing
      beforeEvent = before;
      timeout += FRAMERATE;

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      renderText(TILE, TILE, "SCORE<1>          HI-SCORE");

      char valueFmt[27];
      // score and high-score
      sprintf(valueFmt, "%06d            %06d", score, hiScore);
      renderText(2 * TILE, 3 * TILE, valueFmt);
      // credits
      sprintf(valueFmt, "CREDIT %02d", coins);
      renderText(17 * TILE, HEIGHT - 2 * TILE, valueFmt);

      scene->render(renderer);
      SDL_RenderPresent(renderer);

      scene->update(dt);
    }

    // process scene change, if any
    if (sceneChange) {
      delete scene;
      scene = nextScene;
      nextScene = nullptr;
      sceneChange = false;
    }
  }
}

void run() {
  static bool running = false;
  if (!running) {
    running = true;
    loadAssets();
    mainLoop();
    freeAssets();
  }
}

