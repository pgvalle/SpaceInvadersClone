#pragma once

#include "utils.h"

struct Tourist {
private:
  Timer timer; // for spawn, motion and death

  void update_hidden(Uint64 delta);

  void update_visible(Uint64 delta);

  void update_dying(Uint64 delta);

  void update_dead(Uint64 delta);
  void render_dead(SDL_Renderer* renderer, SDL_Texture* atlas);

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

  void update(Uint64 delta);
  void render(SDL_Renderer* renderer, SDL_Texture* atlas);
};