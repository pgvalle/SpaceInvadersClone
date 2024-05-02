#define APP_INTERNALS

#include "app/App.h"
#include "defines.h"

#include <string>


void changeScene(Scene* newScene) {
  if (nextScene) {
    delete nextScene;
  }

  nextScene = newScene;
  sceneChange = true;
}

bool isKeyPressed(int key) {
  const Uint8 *keys = SDL_GetKeyboardState(nullptr);
  return keys[key];
}

void renderClip(int x, int y, const SDL_Rect &clip) {
  const SDL_Rect scale = {x, y, clip.w, clip.h};
  SDL_RenderCopy(renderer, atlas, &clip, &scale);
}

void renderText(int x, int y, const char *text, SDL_Color color) {
  for (char c : std::string(text)) {
    const SDL_Rect srcRect = {TILE * c, 0, TILE, TILE};
    const SDL_Rect dstRect = {x + TILE * c, y, TILE, TILE};
    SDL_RenderCopy(renderer, texAtlas, &srcRect, &dstRect);
  }
}
