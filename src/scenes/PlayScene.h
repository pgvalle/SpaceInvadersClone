#pragma once

#include "Scene.h"
#include "./play/Shot.h"
#include "./play/UFO.h"
#include "./play/Horde.h"
#include "./play/Bunker.h"
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

  Timer delayer;

  UFO* ufo;
  Timer ufoClock;

  void updateUFO();

  Horde horde;
  Timer hordeShootingClock;

  Bunker bunkers[4];

  Cannon* cannon;
  int cannonLives;
  Timer cannonShootingClock;

  void updatePlayerShooting();

  // dynamic collections

  std::vector<Shot> shots; // collide with stuff and create explosions
  std::vector<Explosion> explosions;

  void updateDynamicCollections();

public:
  PlayScene();
  ~PlayScene();

  void processEvent();
  void update();
  void render();
};
