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

  void update();
  void reset(Uint32 newTimeout = 0);
};
