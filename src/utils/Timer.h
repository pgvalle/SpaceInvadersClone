#pragma once

#include <SDL.h>


class Timer
{
private:
  Uint32 elapsed, timeout;

public:
  Timer(); // default constructor

  bool hasTimedOut() const
  {
    return timeout <= elapsed;
  }

  void update(Uint32 delta);
  void reset(Uint32 newTimeout = 0);
};
