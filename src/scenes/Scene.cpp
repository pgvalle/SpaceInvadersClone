#include "Scene.h"
#include "app/App.h"

bool Scene::processEvent(const SDL_Event &event) {
  if (event.type == SDL_QUIT) {
    closeApp();
    return true;
  }

  return false;
}
