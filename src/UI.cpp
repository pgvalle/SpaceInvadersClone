#include "UI.h"
#include "SIC.h"

void ui_draw() {
  FC_Draw(sic.font, sic.renderer, 8, 8, "YOUR SCORE      HIGH-SCORE");
  FC_Draw(sic.font, sic.renderer, 24, 24, "%06d          %06d", sic.score, sic.high_score);
  FC_Draw(sic.font, sic.renderer, 144, 240, "CREDIT %02d", sic.credits);
}

void ui_update(const SDL_Event &event) {
  const bool up_pressed = (event.type == SDL_KEYDOWN &&
      event.key.keysym.sym == SDLK_UP);

  if (up_pressed)
    sic.credits++;

  const bool down_pressed = (event.type == SDL_KEYDOWN &&
      event.key.keysym.sym == SDLK_DOWN);

  if (down_pressed)
    sic.credits--;
}
