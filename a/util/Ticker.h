#pragma once

// Everything in seconds
class Ticker
{
private:
  int ticksRemaining, totalTicks;
public:
  Ticker(int ticks = 0);

  bool hasFinished() const;
  void tick();
  void reset(int ticks);
  void reset();
};
