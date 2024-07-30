#include "Engine.hpp"
#include "Scene.hpp"
#include "defines.h"
#include <assert.h>

void Engine::init()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  IMG_Init(IMG_INIT_PNG);
  TTF_Init();
}

void Engine::terminate()
{
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

Engine::Engine()
{
  window = SDL_CreateWindow(
    "Space Invaders Clone",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WIDTH, HEIGHT,
    SDL_WINDOW_RESIZABLE
  );
  assert(window);

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  assert(renderer);

  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

  atlas = IMG_LoadTexture(renderer, "./res/atlas.png");
  assert(atlas);
  font = TTF_OpenFont("./res/ps2p.ttf", TILE);
  assert(font);

  time_b4 = SDL_GetTicks();
  time_after = time_b4;

  mainloop();
}

Engine::~Engine()
{
  TTF_CloseFont(font);
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Engine::change_scene(Scene *scene) {

}

void Engine::mainloop() {
  Uint32 delta = 0;
  const Uint32 tgt_delta = 1000 / FPS;

  while (true) {
    time_b4 = time_after;
    time_after = SDL_GetTicks();

    // query and process events
    SDL_Event event;
    if (SDL_WaitEventTimeout(&event, tgt_delta)) {
      if (event.type == SDL_QUIT) return;
      process_events();
    }

    render();
    update();

    const Uint32 real_delta = SDL_GetTicks() - time_b4;
    if (real_delta < tgt_delta) {
      SDL_Delay(tgt_delta - real_delta);
    }    
  }
}

void Engine::process_events() {

}

void Engine::update() {

}

void Engine::render() {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  

  SDL_RenderPresent(renderer);
}

