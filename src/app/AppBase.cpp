#define APP_INTERNALS
// no need to redeclare app fields here!
#define EXT

#include "app/App.h"
#include "defines.h"

#include "scenes/Scene.h"
#include "scenes/MenuScene.h"

#include <cstdio>

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
    SDL_WINDOW_RESIZABLE
  );

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // resolution independent rendering

  // font
  TTF_Font *font = TTF_OpenFont(ASSETS_DIR "ps2p.ttf", TILE);
  
  char ascii[96] = "";
  for (char i = 32; i < 127; i++) {
    ascii[i - 32] = i;
  }

  SDL_Surface *surface = TTF_RenderUTF8_Solid(font, ascii, {255, 255, 255, 255});
  texAtlas = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  TTF_CloseFont(font);

  // image assets
  surface = IMG_Load(ASSETS_DIR "atlas.png");
  atlas = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  surface = SDL_LoadBMP(ASSETS_DIR "icon.bmp");
  SDL_SetWindowIcon(window, surface);
  SDL_FreeSurface(surface);

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

  SDL_DestroyTexture(texAtlas);
  SDL_DestroyTexture(atlas);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void run() {
  // only "one run() in stack"
  static bool running = false;
  if (running) {
    return;
  }

  running = true;
  loadAssets();

  Uint32 eventTimeout = FRAMERATE, before = SDL_GetTicks();

  while (scene) {
    const Uint32 beforeEvent = SDL_GetTicks();

    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, eventTimeout)) {
      scene->processEvent(event);

      // wait less for events next time
      const Uint32 eventDelta = SDL_GetTicks() - beforeEvent;
      eventTimeout -= (eventTimeout >= eventDelta ? eventDelta : eventTimeout);
    }
    else {
      // lapse between last frame starting point and this frame starting point
      const Uint32 delta = SDL_GetTicks() - before;
      before = SDL_GetTicks();
      eventTimeout = FRAMERATE; // again wait for FRAMERATE ms
      // TODO: FRAMERATE < delta. What to do in that case???

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

      scene->update(delta);
    }

    // process scene change, if any
    if (sceneChange) {
      delete scene;
      scene = nextScene;
      nextScene = nullptr;
      sceneChange = false;
    }
  }

  freeAssets();
  running = false;
}
