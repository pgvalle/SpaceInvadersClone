#include "MenuScene.h"
#include "../App.h"

MenuScene::MenuScene() {
  clock.reset(10000);
}

MenuScene::~MenuScene() {
  
}

void MenuScene::processEvent(const SDL_Event &event) {

}

void MenuScene::update(Uint32 delta) {
  clock.update(delta);
  if (clock.hasTimedOut()) {
    shouldClose = true;
  }
}

void MenuScene::render(SDL_Renderer *renderer) {

}
