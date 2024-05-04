// no need to redeclare app fields here!
#define EXTERN
#define APP_INTERNALS

#include "App.h"

#include "scenes/MainScene.h"

#include <cstdio>
#include <cstring>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

/*
 * I don't wanna have everything in a really big function
 */

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
  SDL_RenderSetVSync(renderer, false);
  SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // resolution independent rendering

  // pre-render all ascii characters on a texture
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

  // audio assets

  shouldClose = false;
}

void freeAssets() {
  // audio assets

  // images
  SDL_DestroyTexture(texAtlas);
  SDL_DestroyTexture(atlas);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}

/*
 * here it comes what is in header
 */

void run() {
  // only one run() in stack
  static bool running = false;
  if (running) {
    return;
  }

  running = true;
  loadAssets();
  
  Scene *scene = new MainScene();
  Uint32 eventTimeout = FRAMERATE, before = SDL_GetTicks();

  while (!shouldClose) {
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

      scene->render(renderer);
      scene->update(delta);

      SDL_RenderPresent(renderer);
    }
  }

  delete scene;

  freeAssets();
  running = false;
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

