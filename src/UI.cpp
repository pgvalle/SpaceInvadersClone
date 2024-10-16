#include "UI.h"
#include "SIC.h"

void ui_draw() {
  FC_Draw(sic.font, sic.renderer, 8, 8, "YOUR SCORE      HIGH-SCORE");
  FC_Draw(sic.font, sic.renderer, 24, 24, "%06d          %06d", sic.score, sic.high_score);
  FC_Draw(sic.font, sic.renderer, 144, 240, "CREDIT %02d", sic.credits);
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
