#pragma once

#include "Scene.h"
#include "./play/Horde.h"
#include "./play/UFO.h"
#include "./play/Cannon.h"
#include "./play/Shot.h"


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

  Timer delayer;

  UFO* ufo;
  Timer ufoClock;

  void updateUFO();

  Horde horde;
  Timer hordeShootingClock;

  Cannon* cannon;
  int cannonLives;
  Timer cannonShootingClock;

  void updatePlayerShooting();

  std::vector<Shot> marcelo;

public:
  PlayScene();
  ~PlayScene();

  void processEvent();
  void update();
  void render();
};
