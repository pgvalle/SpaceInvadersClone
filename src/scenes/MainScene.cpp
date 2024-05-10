#include "MainScene.h"
#include "App.h"

#include <cstdio>

MainScene::MainScene() {
  fps = 0;
  credits = 0;
  score = 0;
  hiScore = 0;
}


MainScene::~MainScene() {

}

void MainScene::addToScore(int value) {
  score += value;
  // hiScore needs to keep up with score
  if (score > hiScore) {
    hiScore = score;
  }
}

void MainScene::processEvent(const SDL_Event &event) {
  switch (event.type) {
    case SDL_QUIT:
      shouldClose = true;
      break;

    case SDL_KEYDOWN:
    {
      const SDL_Keycode key = event.key.keysym.sym;
      if (key == SDLK_EQUALS) {
        credits += (credits < 99 ? 1 : 0);
      }
      else if (key == SDLK_MINUS) {
        credits -= (credits > 0 ? 1 : 0);
      }

      break;
    }

    default:
      // TODO: implement credit input
      break;
  }
}

void MainScene::update(float delta) {
  fps = 1 / delta;
}

void MainScene::render(SDL_Renderer *renderer) {
  // clear screen with black color
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  // rendering globals as part of UI

  renderText(8, 8, "SCORE<1>          HI-SCORE");
  // fps
  renderText(10 * 8, 10 * 8, "%.2f", fps);
  // score and high-score
  renderText(2 * 8, 3 * 8, "%06d            %06d", score, hiScore);
  // credits
  renderText(17 * 8, 256 - 2 * 8, "CREDIT %02d", credits);
}

