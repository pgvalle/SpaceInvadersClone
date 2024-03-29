#pragma once

#include <SDL.h>

class Timer {
private:
  Uint32 dt, timeout;

public:
  Timer() {} // default constructor
  Timer(Uint32 timeout);

  bool has_timed_out() const;

  void update();
  void reset(Uint32 new_timeout = 0);
};