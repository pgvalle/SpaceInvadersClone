#ifndef UTIL_TIMER
#define UTIL_TIMER

class Timer
{
private:
  float elapsed = 0, timeout = 0;

public:
  bool hasTimedOut() const;

  void update(float delta);
  void reset(float newTimeout = 0);
};

#endif // UTIL_TIMER
