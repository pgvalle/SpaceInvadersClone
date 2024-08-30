#include "Timer.h"
#include "common.h"

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

void Timer::setTimeoutCallback(Callback cb)
{
  onTimeout = cb;
}

void Timer::update(float dt)
{
  if (elapsed < timeout)
    elapsed += dt;
  else if (!onTimeoutCalled)
  {
    onTimeout();
    onTimeoutCalled = true;
  }
}

void Timer::reset(float newTimeout)
{
  if (newTimeout)
    timeout = newTimeout;
  elapsed = 0;
  onTimeoutCalled = false;
}
