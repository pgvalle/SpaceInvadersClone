#include "UI.h"
#include "SIC.h"

void ui_draw() {

  sic.render_text(144, 240, "CREDIT %d", (Uint8)sic.credits);
  sic.render_text(8,  8, "YOUR SCORE      HIGH-SCORE");
  sic.render_text(8, 24, "%d       %d", sic.score, sic.high_score);
  printf("Hello bro???\n");
}

