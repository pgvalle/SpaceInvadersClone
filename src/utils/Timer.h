#pragma once

#include <SDL.h>


class Timer
{
private:
  Uint32 dt, timeout;

public:
  Timer(); // default constructor

  Timer &operator=(const Timer &other)
  {
    dt = other.dt;
    timeout = other.timeout;
    return *this;
  }

  bool hasTimedOut() const
  {
    return timeout <= dt;
  }

  void update();
  void reset(Uint32 newTimeout = 0);
};
