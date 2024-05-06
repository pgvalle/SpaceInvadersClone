#pragma once

#include <SDL.h>

class Timer {
private:
  Uint32 elapsed = 0, timeout = 0;

public:
  bool hasTimedOut() const {
    return timeout <= elapsed;
  }

  void update(Uint32 delta);
  void reset(Uint32 newTimeout = 0);
};
