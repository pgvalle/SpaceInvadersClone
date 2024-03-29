#include "Horde.h"

Horde::Horde() {
  frozen_timer = Timer(55 * 16);
  state = State::STARTING;
}


void Horde::freeze_temporarily() {
  // timeout will decrease with invader amount decreasing
  const Uint32 new_timeout = invaders.size() * 16;
  frozen_timer.reset(new_timeout);
}


bool Horde::is_finished() {
  return invaders.empty();
}


void Horde::update_starting() {
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


void Horde::update_moving() {
  frozen_timer.update();
  if (frozen_timer.has_timed_out()) {
    int a = xrel, b = 0; // assume moving sideways
    if (++xrel_count == 16) { // should move down
      xrel = -xrel; // next time, go the other 
      xrel_count = 0;
      a = 0;
      b = 8;
    }

    for (Invader& invader : invaders) {
      invader.move(a, b);
    }
    
    frozen_timer.reset();
  }
}


void Horde::update() {
  switch (state) {
  case State::STARTING:
    update_starting();
    break;
  case State::MOVING:
    update_moving();
    break;
  }
}

void Horde::render() {
  for (Invader& invader : invaders) {
    invader.render();
  }
}