#ifndef UTIL_TIMER
#define UTIL_TIMER

class Timer {
private:
  float elapsed = 0, timeout = 0;

public:
  bool has_timed_out() const {
    return timeout <= elapsed;
  }

  void update(float delta);
  void reset(float new_timeout = 0);
};

#endif  // UTIL_TIMER
