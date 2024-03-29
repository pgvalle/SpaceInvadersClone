#pragma once

#include <SDL.h>


class Timer
{
private:
  Uint32 dt, timeout;

public:
  Timer(); // default constructor

  bool hasTimedOut() const;

  void update();
  void reset(Uint32 newTimeout = 0);
};