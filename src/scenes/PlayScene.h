#pragma once

#include "Scene.h"
#include "entities/Shot.h"
#include "entities/UFO.h"
#include "entities/Horde.h"
#include "entities/Bunker.h"
#include "entities/Cannon.h"


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

  UFO ufo;

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

  void updateShotsAndCollisions();
  void updateExplosions();

public:
  PlayScene();
  ~PlayScene();

  void processEvent(const SDL_Event &event);
  void update(Uint32 delta);
  void render(SDL_Renderer *renderer);
};
