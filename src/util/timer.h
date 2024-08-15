#ifndef UTIL_TIMER
#define UTIL_TIMER

class Timer
{
private:
  float elapsed = 0, timeout = 0;

public:
  static float getRandomTimeout(float min, float max);

  bool hasTimedOut() const;

  void update();
  void reset(float newTimeout = 0);
};

#endif // UTIL_TIMER
