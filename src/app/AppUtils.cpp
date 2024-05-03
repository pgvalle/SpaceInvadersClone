#define APP_INTERNALS

#include "app/App.h"
#include "defines.h"

#include <cstring>

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
  for (int i = 0; i < (int)strlen(text); i++) {
    const char c = text[i] - 32; // printable characters only
    const SDL_Rect srcRect = {TILE * c, 0, TILE, TILE};
    const SDL_Rect dstRect = {x + TILE * i, y, TILE, TILE};

    SDL_RenderCopy(renderer, texAtlas, &srcRect, &dstRect);
  }
}
