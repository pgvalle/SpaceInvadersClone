#include "UI.h"
#include "SIC.h"

void ui_init() {

}

void ui_draw() {
  sic.draw_text(8, 8, "YOUR SCORE      HIGH-SCORE");
  sic.draw_text(24, 24, "%06d          %06d", sic.score, sic.high_score);
  sic.draw_text(144, 240, "CREDIT %02d", sic.credits);
}

void ui_update(const SDL_Event &event) {
  bool pressed = (event.type == SDL_KEYDOWN);
  SDL_Keycode key = event.key.keysym.sym;

  if (pressed && key == SDLK_UP) {
    if (sic.credits < 99)
      sic.credits++;
  }

  if (pressed && key == SDLK_DOWN) {
    if (sic.credits > 0)
      sic.credits--;
  }
}
