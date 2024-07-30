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

Engine::Engine(Scene *scene)
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

  current_scene = scene;
  next_scene = nullptr;
}

Engine::~Engine()
{
  TTF_CloseFont(font);
  SDL_DestroyTexture(atlas);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

void Engine::change_scene(Scene *scene) {
  next_scene = scene;
}

void Engine::request_exit() {
  SDL_QuitEvent evt;
  evt.type = SDL_QUIT;
  evt.timestamp = SDL_GetTicks();
  SDL_PushEvent((SDL_Event *)&evt);
}

void Engine::mainloop() {
  Uint32 delta = 0;
  const Uint32 tgt_delta = 1000 / FPS;

  while (current_scene) {
    time_b4 = time_after;
    time_after = SDL_GetTicks();

    // query and process events
    SDL_Event evt;
    if (SDL_WaitEventTimeout(&evt, tgt_delta)) {
      if (evt.type == SDL_QUIT) return;
      current_scene->process_event(this, evt);
    }

    current_scene->update(this);
    current_scene->render(this);
    SDL_RenderPresent(renderer);

    if (next_scene) {
      delete current_scene;
      current_scene = next_scene;
      next_scene = nullptr;
    }

    const Uint32 real_delta = SDL_GetTicks() - time_b4;
    if (real_delta < tgt_delta) {
      SDL_Delay(tgt_delta - real_delta);
    }
  }
}
