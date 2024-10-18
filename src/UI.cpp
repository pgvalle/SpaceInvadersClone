#include "UI.h"
#include "SIC.h"

void ui_draw() {
  sic.render_text(8,  8, "YOUR SCORE      HIGH-SCORE");
  sic.render_text(8, 24, "%06d       %06d", sic.score, sic.high_score);

  sic.render_text(144, 240, "CREDIT %02d", sic.credits);
}

