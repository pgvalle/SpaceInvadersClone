#include "SIC.h"
#include "UI.h"

static int ticks;
static bool cover;

void ready_init() {
  ticks = 0;
  cover = false;
}

void ready_draw() {
  ui_draw();

  if (ticks % 7 == 0)
    cover = !cover;

  if (!cover)
    return;

  SDL_Rect rect = { 24, 24, 48, 8 };

  SDL_SetRenderDrawColor(sic.renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(sic.renderer, &rect);
}

int ready_update(const SDL_Event &event) {
  ui_update(event);

  ticks++;

  bool pressed_no_repeat = (event.type == SDL_KEYDOWN && !event.key.repeat);
  SDL_Keycode key = event.key.keysym.sym;
  
  if (pressed_no_repeat && key == SDLK_RETURN) {
    sic.credits--;
    return 2;
  }

  if (pressed_no_repeat && key == SDLK_q)
    return SCREEN_EXIT_HOOK;

  return SCREEN_UNCHANGED;
}

