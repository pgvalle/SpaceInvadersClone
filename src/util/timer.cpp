#include "timer.h"
#include "globals.h"

float Timer::getRandomTimeout(float min, float max)
{
  const float normalizedRand = (float)rand() / RAND_MAX;
  const float intervalSize = std::max(min, max) - std::min(min, max);
  return normalizedRand * intervalSize + std::min(min, max);
}

bool Timer::hasTimedOut() const
{
  return timeout <= elapsed;
}

void Timer::update()
{
  if (elapsed < timeout)
    elapsed += 1.0 / FPS;
}

void Timer::reset(float newTimeout)
{
  elapsed = 0;
  if (newTimeout)
    timeout = newTimeout;
}
