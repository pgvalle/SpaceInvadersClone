#ifndef _BUNKER_
#define _BUNKER_

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
  void render(SDL_Renderer *renderer) const;
};


#endif /* _BUNKER_ */
