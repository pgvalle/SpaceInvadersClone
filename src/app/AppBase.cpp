#include "app/App.h"
#include "defines.h"

#include "scenes/Scene.h"
#include "scenes/MenuScene.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>


App *app = nullptr;

void App::start()
{
  // only run if not run before
  if (app) return;

  app = new App();
  app->mainLoop();
  delete app;
}


App::App()
{
  scene = new MenuScene();
  nextScene = nullptr;
  score = 0;
  hiScore = 0; // load from file
  coins = 0; // load from file
  dt = 0;

  event.type = 0;

  window = SDL_CreateWindow(
      "Space Invaders Clone",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      2 * WIDTH,
      2 * HEIGHT,
      SDL_WINDOW_RESIZABLE);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // resolution independent rendering

  loadAssets();
}

App::~App()
{
  freeAssets();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  // save hiScore and coins in a file
}


void App::loadAssets()
{
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

  // audio assets
}

void App::freeAssets()
{
  // audio assets

  SDL_DestroyTexture(atlas);

  TTF_CloseFont(font);
}
