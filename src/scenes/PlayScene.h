#pragma once

#include "Scene.h"
#include "./play/Horde.h"
#include "./play/UFO.h"
#include "./play/Cannon.h"


class PlayScene : public Scene
{
private:
  enum State
  {
    STARTING,
    PLAYING,
    DELAYING,
    PAUSED,
    UNPAUSING
  } state;

  Timer delayer; // delay after player death.

  UFO* ufo;
  Timer ufoClock;

  Horde horde;
  Timer hordeShootingClock;

  Cannon* cannon;
  int cannonLives;
  Timer cannonShootingClock;

public:
  PlayScene();
  ~PlayScene();

  void processEvent();
  void update();
  void render();
};
