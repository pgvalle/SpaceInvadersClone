#ifndef UTIL_TIMER
#define UTIL_TIMER

#include "common.h"

// Everything in seconds
class Timer
{
private:
  float elapsed, timeout;
  bool onTimeoutCalled;

public:
  Callback onTimeout;

  static float getRandomTime(float min, float max);

  Timer(float timeout = 0);

  float getTimeout() const
  {
    return timeout;
  }

  void update(float dt);
  void reset(float newTimeout = 0);
};

#endif // UTIL_TIMER
