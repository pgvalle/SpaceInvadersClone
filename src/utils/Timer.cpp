#include "app/App.h"
#include "Timer.h"

Timer::Timer()
{
  elapsed = 0;
  timeout = 0;
}

void Timer::update(Uint32 delta)
{
  if (elapsed < timeout)
  {
    elapsed += delta;
  }
}

void Timer::reset(Uint32 newTimeout)
{
  elapsed = 0;
  if (newTimeout != 0)
  {
    timeout = newTimeout;
  }
}
