#include "Timer.hpp"

void Timer::update(float delta) {
  if (elapsed < timeout) {
    elapsed += delta;
  }
}

void Timer::reset(float new_timeout) {
  elapsed = 0;
  if (new_timeout) {
    timeout = new_timeout;
  }
}
