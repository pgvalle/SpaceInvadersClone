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

  dt = 0;
  event.type = 0;

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


bool running = false;

void run() {
  if (!running) {
    running = true;
    loadAssets();
    mainLoop();
    freeAssets();
  }
}
