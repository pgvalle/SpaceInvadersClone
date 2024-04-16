#include "app/App.h"
#include "Timer.h"

Timer::Timer()
{
  elapsed = 0;
  timeout = 0;
}

void Timer::update()
{
  if (elapsed < timeout)
  {
    elapsed += dt;
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
