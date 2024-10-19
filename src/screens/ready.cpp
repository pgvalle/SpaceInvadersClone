#include "SIC.h"
#include "UI.h"

static int ticks;
static bool show_score;

void ready_init() {
  ticks = 0;
  show_score = false;
}

void ready_draw() {
  ui_draw();

  if (show_score)
    return;

  SDL_Rect rect = { 24, 24, 48, 8 };

  SDL_SetRenderDrawColor(sic.renderer, 0, 0, 0, 255);
  SDL_RenderFillRect(sic.renderer, &rect);
}

int ready_update(const SDL_Event &event) {
  ui_update(event);

  if (ticks++ % 3 == 0)
    show_score = !show_score;

  bool pressed_no_repeat = (event.type == SDL_KEYDOWN && !event.key.repeat);
  SDL_Keycode key = event.key.keysym.sym;
  
  if (pressed_no_repeat && key == SDLK_RETURN) {
    sic.credits--;
    return PLAY_SCREEN;
  }

  if (pressed_no_repeat && key == SDLK_q)
    return NULL_SCREEN;

  return READY_SCREEN;
}

