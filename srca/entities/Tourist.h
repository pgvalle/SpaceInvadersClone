#pragma once

#include "utils/Timer.h"

struct Tourist {
private:
  Timer timer; // for spawn, motion and death

  void update_hidden();

  void update_visible();

  void update_dying();

  void update_dead();
  void render_dead();

public:
  enum State {
    VISIBLE,
    HIDDEN,
    DYING, // showing death animation
    DEAD, // show its score value
    AFTERLIFE // they are in a better place
  } state;
  int x, xvel;
  int score_value;

  Tourist();

  void process_strike();

  void update();
  void render();
};