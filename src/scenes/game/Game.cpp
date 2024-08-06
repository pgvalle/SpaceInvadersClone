#include "Game.h"
#include <core/Engine.hpp>

Game::Game() {
  
}

Game::~Game() {

}

void Game::process_event(Engine *eng, const SDL_Event &evt) {
  
}

void Game::update(Engine *eng) {

}

void Game::render(const Engine *eng) {
  SDL_SetRenderDrawColor(eng->renderer, 0, 0, 0, 255);
  SDL_RenderClear(eng->renderer);
}
