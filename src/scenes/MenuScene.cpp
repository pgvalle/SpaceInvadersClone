#include "MenuScene.h"
#include "App.h"

MenuScene::MenuScene() {
  clock.reset(10);
}

MenuScene::~MenuScene() {
  
}

void MenuScene::processEvent(const SDL_Event &event) {

}

void MenuScene::update(float delta) {
  clock.update(delta);
  if (clock.hasTimedOut()) {
    shouldClose = true;
  }
}

void MenuScene::render(SDL_Renderer *renderer) {

}
