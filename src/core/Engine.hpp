#ifndef CORE_ENGINE
#define CORE_ENGINE

#include "Scene.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

class Engine {
public:
  static void init();
  static void terminate();

  Engine();
  ~Engine();

  // Engine API for scenes

  float get_delta_sec() const {
    return 1e-3 * (time_after - time_b4);
  }

  void change_scene(Scene *scene);

  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *atlas;
  TTF_Font *font;

private:
  Uint32 time_b4, time_after;
  Scene *current_scene, *next_scene;

  // game loop stuff

  void mainloop();

  void process_events();
  void update();
  void render();
};

#endif  // CORE_ENGINE
