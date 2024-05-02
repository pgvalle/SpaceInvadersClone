#include "Scene.h"
#include "app/App.h"

bool Scene::processEvent(const SDL_Event &event) {
  if (event.type == SDL_QUIT) {
    changeScene(nullptr);
    return true;
  }

  return false;
}
