#pragma once

#include <SDL.h>


class Timer
{
private:
  Uint32 dt, timeout;

public:
  Timer(); // default constructor

  bool hasTimedOut() const
  {
    return timeout <= dt;
  }

  void update();
  void reset(Uint32 newTimeout = 0);
};
