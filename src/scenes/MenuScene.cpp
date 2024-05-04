#include "app/App.h"
#include "MenuScene.h"
//#include "PlayScene.h"

#include <string.h>


MenuScene::MenuScene() {
  clock.reset(10000);
}

MenuScene::~MenuScene() {
  
}

/*
bool MenuScene::processEvent(const SDL_Event &event) {

}
*/

void MenuScene::update(Uint32 delta) {
  clock.update(delta);
  if (clock.hasTimedOut()) {
    closeApp();
  }
}

void MenuScene::render(SDL_Renderer *renderer) {

}
