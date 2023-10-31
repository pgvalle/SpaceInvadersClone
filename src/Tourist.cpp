#include "Tourist.h"

Tourist::Tourist() {
  timer = Timer(15000); // spawn

  state = State::HIDDEN;
  x = -10000;
  xvel = 0;
  score_value = 100;
}

void Tourist::process_strike() {
  state = State::DYING;
  timer.reset(120);
}

void Tourist::update_hidden(Uint64 delta) {
  timer.update(delta);
  if (timer.has_timed_out()) {
    state = State::VISIBLE;
    // pick a random side to appear
    const bool left = rand() % 2;
    x = left ? 0 : 200;
    xvel = left ? 1 : -1;
    // will be used as motion timer
    timer.reset(60);
  }
}

void Tourist::update_visible(Uint64 delta) {
  // useful to make it move slower than what the framerate permits
  timer.update(delta);
  if (timer.has_timed_out()) {
    x += xvel;
  }
}

void Tourist::update_dying(Uint64 delta) {
  // animation frame should change
  timer.update(delta);
  if (timer.has_timed_out()) {
    state = State::DEAD; // now we show it's score value
    timer.reset(1000);
  }
}

void Tourist::update_dead(Uint64 delta) {
  timer.update(delta);
  if (timer.has_timed_out()) {
    state = State::AFTERLIFE;
  }
}

void Tourist::render_dead(SDL_Renderer* renderer, SDL_Texture* atlas) {
  
}

void Tourist::update(Uint64 delta) {
  switch (state) {
  case State::HIDDEN:
    update_hidden(delta);
    break;
  case State::VISIBLE:
    update_visible(delta);
    break;
  case State::DYING:
    update_dying(delta);
    break;
  case State::DEAD:
    update_dead(delta);
    break;
  }
}

void Tourist::render(SDL_Renderer* renderer, SDL_Texture* atlas) {
  switch (state) {
  case State::HIDDEN:
    break;
  case State::VISIBLE:
    render_clip(x, 20, { 4, 0, 16, 8 }, renderer, atlas);
    break;
  case State::DYING:
    render_clip(x - 4, 20, { 24, 0, 24, 8 }, renderer, atlas);
    break;
  case State::DEAD:
    render_dead(renderer, atlas);
    break;
  case State::AFTERLIFE:
    break;
  }
}