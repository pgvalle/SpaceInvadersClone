#include "Cannon.h"
#include "App.h"

Cannon::Cannon() {
  death_anim = Animation({ 0, 8, 16, 8 }, 112, 2);

  state = State::STARTING;
  x = 16;
  lives = 3;
  timer = Timer(2000); // 2 seconds
}


void Cannon::update() {
  switch (state) {
  case State::STARTING:
    timer.update();
    if (timer.has_timed_out()) {
      state = State::ALIVE;
      timer.reset(768); // for shooting mechanic
    }
    break;
  case State::ALIVE:
    // movement
    if (App::key_pressed(SDL_SCANCODE_LEFT)) {
      x -= (x > 14 ? 1 : 0);
    }
    if (App::key_pressed(SDL_SCANCODE_RIGHT)) {
      x += (x < App::world_width() - 31 ? 1 : 0);
    }
    // shooting mechanic (actual shooting handled outside)
    timer.update();
    break;
  case State::DYING:
    timer.update();
    if (timer.has_timed_out()) {
      state = State::DEAD;
      timer.reset(2000); // dead for 2 seconds
    } else {
      death_anim.update();
    }
    break;
  case State::DEAD:
    timer.update();
    if (timer.has_timed_out()) {
      // set state variables
      lives--;
    }
    break;
  }
}

void Cannon::render() {

}
