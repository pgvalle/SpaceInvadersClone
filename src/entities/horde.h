#ifndef ENTITIES_HORDE
#define ENTITIES_HORDE

#include <common.h>
#include "explosion.h"

struct Invader
{
  int type;
  int animationFrame;
  int x, y;

  Invader(int row, int col);

  int getScoreValue() const;
  SDL_Rect getRect() const;

  void move(int xOff, int yOff);
  void render() const;
};

struct Horde
{
  Timer clock;
  int x, vx;
  bool populating;
  std::vector<Invader> invaders;

  Horde();

  Explosion *onHit(const SDL_Rect &rect);
  void update();
  void render() const;
};

extern Horde horde;

#endif  // ENTITIES_HORDE
