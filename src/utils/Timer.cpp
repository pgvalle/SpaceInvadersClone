#include "app/App.h"
#include "Timer.h"

Timer::Timer()
{
  dt = 0;
  timeout = 0;
}

bool Timer::hasTimedOut() const
{
  return timeout <= dt;
}

void Timer::update()
{
  if (dt < timeout)
  {
    dt += app->dt;
  }
}

void Timer::reset(Uint32 newTimeout)
{
  dt = 0;
  if (newTimeout != 0)
  {
    timeout = newTimeout;
  }
}
