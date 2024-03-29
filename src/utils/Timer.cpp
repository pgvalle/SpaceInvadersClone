#include "../core.h"

Timer::Timer(Uint32 timeout) {
  dt = 0;
  timeout = timeout;
}

bool Timer::has_timed_out() const {
  return timeout <= dt;
}

void Timer::update() {
  if (dt < timeout) {
    dt += ctx::dt;
  }
}

void Timer::reset(Uint32 new_timeout) {
  dt = 0;
  if (new_timeout != 0) {
    timeout = new_timeout;
  }
}
