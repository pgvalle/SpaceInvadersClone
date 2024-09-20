#include "Timer.h"

Timer::Timer(float $timeout)
{
  reset($timeout);
  onTimeout = []()
  {
    printf("Timer timed out!\n");
  };
}

float Timer::getRandomTime(float min, float max)
{
  assert(min <= max);

  const float from0to1 = (float)rand() / RAND_MAX;
  return from0to1 * (max - min) + min;
}

void Timer::update(float dt)
{
  if (elapsed < timeout)
    elapsed += dt;
  else if (!onTimeoutCalled)
  {
    onTimeoutCalled = true;
    onTimeout();
  }
}

void Timer::reset(float newTimeout)
{
  if (newTimeout)
    timeout = newTimeout;
  elapsed = 0;
  onTimeoutCalled = false;
}
