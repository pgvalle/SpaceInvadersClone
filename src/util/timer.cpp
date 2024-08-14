#include "timer.h"

bool Timer::hasTimedOut() const
{
  return timeout <= elapsed;
}

void Timer::update(float delta)
{
  if (elapsed < timeout)
    elapsed += delta;
}

void Timer::reset(float newTimeout)
{
  elapsed = 0;
  if (newTimeout)
    timeout = newTimeout;
}
