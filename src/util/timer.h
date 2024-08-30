#ifndef UTIL_TIMER
#define UTIL_TIMER

#include <functional>

// Everything in seconds
class Timer
{
public:
  using Callback = std::function<void()>;

private:
  float elapsed, timeout;
  bool onTimeoutCalled;
  Callback onTimeout;

public:
  static float getRandomTime(float min, float max);

  Timer(float timeout = 0);

  void setTimeoutCallback(Callback cb);

  void update(float dt);
  void reset(float newTimeout = 0);
};

#endif // UTIL_TIMER
