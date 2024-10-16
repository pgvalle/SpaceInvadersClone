#include "Ticker.h"
#include "common.h"

Ticker::Ticker(int ticks)
{
  reset(ticks);
}

bool Ticker::hasFinished() const
{
  return ticksRemaining == 0;
}

void Ticker::tick()
{
  if (ticksRemaining > 0)
    ticksRemaining--;
}

void Ticker::reset(int ticks)
{
  ticksRemaining = ticks;
  totalTicks = ticks;
}

void Ticker::reset()
{
  ticksRemaining = totalTicks;
}
