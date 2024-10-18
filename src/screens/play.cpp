#include "SIC.h"
#include "UI.h"

void play_init() {

}

void play_draw() {
  ui_draw();
}

int play_update(const SDL_Event &event) {
  ui_update(event);

  bool pressed_no_repeat = (event.type == SDL_KEYDOWN && event.key.repeat == 0);
  SDL_Keycode key = event.key.keysym.sym;

  if (pressed_no_repeat && key == SDLK_q)
    return SCREEN_EXIT_HOOK;

  return SCREEN_UNCHANGED;
}
