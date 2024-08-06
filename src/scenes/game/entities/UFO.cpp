#include "UFO.h"

#define Y (5 * 8)

UFO::UFO() {
  // start at away state
  state = AWAY;
  clock.reset(10); // spawn
}

bool UFO::checkAndProcessHit(const SDL_Rect &hitbox) {
  // not even there to be hit
  if (state != ALIVE) return false;

  const SDL_Rect ufoHB = {(int)roundf(x) + 4, Y, 16, 8};
  const bool hit = SDL_HasIntersection(&hitbox, &ufoHB);
  if (hit) {
    // transition to dying1 state
    state = DYING1;
    clock.reset(0.3);
  }

  return hit;
}

void UFO::update(float delta) {
  switch (state) {
    case AWAY:
      clock.update(delta);
      if (clock.has_timed_out()) {
        state = ALIVE;
        // randomly choose a corner
        left = rand() % 2;
        x = left ? (24 * 8) : 16;
        // randomly choose a score value
        scoreValue = 100;
      }
      break;

    case ALIVE: {
      const float x_vel = 40.0f * (left ? -1 : 1);  // 1 canvas_unit/s
      const float dx = x_vel * delta;
      
      x += dx;
      const int xi = roundf(x);
      if (xi < 16 || xi > 24 * 8) { // out of bounds
        // transition to away state
        state = AWAY;
        clock.reset(10);
      }
      break;
    }

    case DYING1:
      // animation frame should change
      clock.update(delta);
      if (clock.has_timed_out()) {
        state = DYING2;  // now we show it's score value
        clock.reset(2);
        // sum score value
        
      }
      break;

    case DYING2:
      clock.update(delta);
      if (clock.has_timed_out()) {
        // transition to away state
        state = AWAY;
        clock.reset(10);
      }
      break;

    default:
      break;
  }
}

void UFO::render() const
{
  const int xi = roundf(x);
  switch (state)
  {
    case ALIVE:
      NAGE::renderTile(xi, Y, 0);
      NAGE::setFlip(SDL_FLIP_HORIZONTAL);
      NAGE::renderTile(xi + 8, Y, 0);
      NAGE::setFlip(SDL_FLIP_NONE);
      break;
    case DYING1:
      NAGE::renderTile(xi, Y, 1);
      NAGE::renderTile(xi + 8, Y, 2);
      NAGE::renderTile(xi + 16, Y, 3);
      break;
    case DYING2:
      NAGE::renderText(xi, Y, "%3d", scoreValue);
      break;
    default:
      break;
  }
}
