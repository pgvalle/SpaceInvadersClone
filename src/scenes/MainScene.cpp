#include "MainScene.h"
#include "../App.h"

MainScene::MainScene() {
  coins = 0;
  score = 0;
  hiScore = 0;
}


MainScene::~MainScene() {

}

void MainScene::processEvent(const SDL_Event &event) {
  switch (event.type) {
    case SDL_QUIT: {
      shouldClose = true;
      break;
    }

    default: {
      printf("Event not processed...\n");
      // TODO: implement credit input
    }
  }
}

void MainScene::update(Uint32 delta) {

}

void MainScene::render(SDL_Renderer *renderer) {
  renderText(TILE, TILE, "SCORE<1>          HI-SCORE");

  static char valueFmt[27];
  // score and high-score
  sprintf(valueFmt, "%06d            %06d", score, hiScore);
  renderText(2 * TILE, 3 * TILE, valueFmt);
  // credits
  sprintf(valueFmt, "CREDIT %02d", coins);
  renderText(17 * TILE, HEIGHT - 2 * TILE, valueFmt);
}

