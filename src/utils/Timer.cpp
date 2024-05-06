#include "Timer.h"

void Timer::update(Uint32 delta) {
  if (elapsed < timeout) {
    elapsed += delta;
  }
}

void Timer::reset(Uint32 newTimeout) {
  elapsed = 0;
  if (newTimeout) {
    timeout = newTimeout;
  }
}
