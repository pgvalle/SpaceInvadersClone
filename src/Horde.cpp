#include "Horde.h"

Horde::Horde() {
  motion_timer = Timer(55 * 16);
  state = State::STARTING;
}

void Horde::freeze_temporarily() {
  // timeout will decrease with invader amount decreasing
  const Uint64 new_timeout = invaders.size() * 16;
  motion_timer.reset(new_timeout);
}

void Horde::update_starting(Uint64 delta) {
  if (invaders.size() == 55) {
    // done. Now start moving
    state = State::MOVING;
    xrel = 2;
    xrel_count = 8;
  } else {
    const int row = 4 - invaders.size() / 11;
    const int col = invaders.size() % 11;
    invaders.push_back(Invader(row, col));
  }
}

void Horde::update_moving(Uint64 delta) {
  motion_timer.update(delta);
  if (motion_timer.has_timed_out()) {
    // Should swap directions. But before that, move down.
    if (++xrel_count == 16) {
      xrel = -xrel;
      for (Invader& invader : invaders) {
        invader.move(0, 8);
      }
    } else {
      for (Invader& invader : invaders) {
        invader.move(xrel, 0);
      }
    }
    
    motion_timer.reset();
  }
}

void Horde::update(Uint64 delta) {
  switch (state) {
  case State::STARTING:
    update_starting(delta);
    break;
  case State::MOVING:
    update_moving(delta);
    break;
  }
}

void Horde::render(SDL_Renderer* renderer, SDL_Texture* atlas) {
  for (Invader& invader : invaders) {
    invader.render(renderer, atlas);
  }
}