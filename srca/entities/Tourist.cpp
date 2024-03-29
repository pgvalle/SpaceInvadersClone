#include "Tourist.h"
#include "App.h"

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


void Tourist::update_hidden() {
  timer.update();
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


void Tourist::update_visible() {
  // useful to make it move slower than what the framerate permits
  timer.update();
  if (timer.has_timed_out()) {
    x += xvel;
  }
}


void Tourist::update_dying() {
  // animation frame should change
  timer.update();
  if (timer.has_timed_out()) {
    state = State::DEAD; // now we show it's score value
    timer.reset(2000);
  }
}


void Tourist::update_dead() {
  timer.update();
  if (timer.has_timed_out()) {
    state = State::AFTERLIFE;
  }
}

void Tourist::render_dead() {
  // TODO: implement rendering tourist score
}


void Tourist::update() {
  switch (state) {
  case State::HIDDEN:
    update_hidden();
    break;
  case State::VISIBLE:
    update_visible();
    break;
  case State::DYING:
    update_dying();
    break;
  case State::DEAD:
    update_dead();
    break;
  case State::AFTERLIFE:
    break;
  }
}

void Tourist::render() {
  switch (state) {
  case State::HIDDEN:
    break;
  case State::VISIBLE:
    App::render_clip(x, 20, { 4, 0, 16, 8 });
    break;
  case State::DYING:
    App::render_clip(x - 4, 20, { 24, 0, 24, 8 });
    break;
  case State::DEAD:
    render_dead();
    break;
  case State::AFTERLIFE:
    break;
  }
}