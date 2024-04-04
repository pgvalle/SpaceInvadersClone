#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"

#include <stdio.h>


PlayScene::PlayScene()
{
  state = STARTING;
  delayer.reset(2500);

  cannon = nullptr;
  cannonLives = 3;
  cannonShootingClock.reset(1000);
}

PlayScene::~PlayScene()
{
  
}


void PlayScene::processEvent()
{
}

void PlayScene::update()
{
  switch (state)
  {
  case STARTING:
    horde.update();

    delayer.update();
    if (delayer.hasTimedOut())
    {
      state = PLAYING;
      cannon = new Cannon();
    }

    break; 
  case PLAYING:
    // update these first
    updateExplosions();
    updateShotsAndCollisions();

    updatePlayerShooting();
    ufo.update();

    horde.update();

    cannon->update();
    if (cannon->checkAndProcessHit({WIDTH - 5 * TILE, HEIGHT - 5 * TILE, 20, 8}))
    {
      state = DELAYING;
    }

    break;
  case DELAYING:
    if (cannon->isDead())
    {
      delayer.update();
      if (!delayer.hasTimedOut()) break;
      
      state = PLAYING;
      delete cannon;
      cannon = new Cannon();      
    }
    else
    {
      // not dead but may be dead after update
      cannon->update();
      if (!cannon->isDead()) break;

      delayer.reset(2000); // wait 3 seconds after cannon death
      cannonLives--;
    }

    break;
  default:
    break;
  }
}

void PlayScene::render()
{
  for (Bunker& bunker : bunkers)
  {
    bunker.render();
  }

  ufo.render();
  horde.render();
  if (cannon) cannon->render();

  for (const Shot& shot : shots)
  {
    shot.render();
  }

  for (const Explosion& explosion : explosions)
  {
    explosion.render();
  }

  const int cannonsY = HEIGHT - 2 * TILE;
  for (int i = 0; i < cannonLives - 1; i++)
  {
    app->renderClip(3 * TILE + 16 * i, cannonsY, {0, 8, 16, 8});
  }

  char livesFmt[2];
  livesFmt[0] = (char)cannonLives + '0';
  app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);
}
