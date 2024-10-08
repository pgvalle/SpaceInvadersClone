#pragma once

// Everything in seconds
class Timer
{
private:
  float elapsed, timeout;

public:
  static float getRandomTime(float min, float max);

  Timer(float timeout = 0);

  bool hasTimedOut() const;
  float getTimeout() const;
  void update(float dt);
  void reset(float newTimeout = 0);
};
