#include "Timer.h"
#include "common.h"

Timer::Timer(float $timeout)
{
  reset($timeout);
}

float Timer::getRandomTime(float min, float max)
{
  assert(min <= max);

  const float from0to1 = (float)rand() / RAND_MAX;
  return from0to1 * (max - min) + min;
}

bool hasTimedOut() const
{
  return elapsed >= timeout;
}

float getTimeout() const
{
  return timeout;
}

void Timer::update(float dt)
{
  if (elapsed < timeout)
    elapsed += dt;
}

void Timer::reset(float newTimeout)
{
  if (newTimeout)
    timeout = newTimeout;
  elapsed = 0;
}
