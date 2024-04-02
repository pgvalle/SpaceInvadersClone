#pragma once

#include <SDL.h>


#define BUNKER_Y 192

class Bunker
{
private:
  SDL_Rect outerBounds;
  bool bits[352];

public:
  Bunker();

  bool checkAndProcessHit(const SDL_Rect &hitbox);
  void render() const;
};
