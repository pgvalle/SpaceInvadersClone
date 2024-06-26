#ifndef _UFO_
#define _UFO_

#include <NAGE.h>

struct UFO
{
private:
  enum State
  {
    AWAY,
    ALIVE,
    DYING1, // showing death animation
    DYING2, // show its score value
    DEAD    // they are in a better place
  } state;

  NAGE::Timer clock;
  bool left;
  float x;
  int scoreValue;

public:
  UFO();

  bool checkAndProcessHit(const SDL_Rect &hitbox);

  void update(float delta);
  void render() const;
};


#endif /* _UFO_ */
