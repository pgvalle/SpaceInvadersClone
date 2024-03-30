#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"

#include <stdio.h>


void PlayScene::updateUFO()
{
  if (ufo)
  {
    ufo->update();
    if (!ufo->isDead()) return;

    delete ufo;
    ufo = nullptr;
    ufoClock.reset(5000);
  }
  else
  {
    ufoClock.update();
    if (!ufoClock.hasTimedOut()) return;

    ufo = new UFO();
  }
}

void PlayScene::updatePlayerShooting()
{
  if (!cannonShootingClock.hasTimedOut())
  {
    cannonShootingClock.update();
  }
  else if (app->isKeyPressed(SDL_SCANCODE_SPACE))
  {
    shots.push_back(cannon->shoot());
    cannonShootingClock.reset();
  }
}

PlayScene::PlayScene()
{
  state = STARTING;
  delayer.reset(2500);

  ufo = nullptr;
  ufoClock.reset(5000);

  cannon = nullptr;
  cannonLives = 4;
  cannonShootingClock.reset(1500);
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
      cannonLives--;
    }

    break; 
  case PLAYING:
    updatePlayerShooting();
    updateUFO();

    horde.update();

    cannon->update();
    if (cannon->checkAndProcessHit({WIDTH - 5 * TILE, HEIGHT - 5 * TILE, 20, 8}))
    {
      state = DELAYING;
    }

    for (size_t i = 0; i < explosions.size(); i++)
    {
      explosions[i].update();
      if (explosions[i].hasFinished())
      {
        explosions.erase(explosions.begin() + i--);
        continue;
      }
    }

    // shots update and collision checks
    for (size_t i = 0; i < shots.size(); i++)
    {
      Shot& shot = shots[i];
      const SDL_Rect shotRect = { shot.x, shot.y, 1, 8 };
      shot.update();

      // collision with horde
      Explosion explosion = horde.checkAndProcessHit(shotRect);
      explosions.push_back(explosion);
      if (!explosion.hasFinished()) // valid explosion. Collision occurred
      {
        shots.erase(shots.begin() + i--);
        continue;
      }

      // collision with ufo
      if (ufo && ufo->checkAndProcessHit(shotRect))
      {
        shots.erase(shots.begin() + i--);
        continue;
      }

      // collision with player


      // collision with other shot

    }

    break;
  case DELAYING:
    if (cannon)
    {
      cannon->update();
      if (cannon->isDead())
      {
        delete cannon;
        cannon = nullptr;
        delayer.reset(3000); // wait 3 seconds after cannon death
      }
    }
    else
    {
      delayer.update();
      if (delayer.hasTimedOut())
      {
        state = PLAYING;
        cannon = new Cannon();
        cannonLives--;
      }
    }

    break;
  }
}

void PlayScene::render()
{
  horde.render();

  if (ufo) ufo->render();
  if (cannon) cannon->render();

  for (const Shot& shot : shots)
  {
    shot.render();
  }

  for (const Explosion& explosion : explosions)
  {
    explosion.render();
  }

  switch (state)
  {
  char livesFmt[2];

  case STARTING:
    // rendered here so that we don't actually start displaying 4
    livesFmt[0] = (char)(cannonLives - 1) + '0';
    app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);

    break;
  default:
    livesFmt[0] = (char)cannonLives + '0';
    app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);
  }

  // render cannons
  const int cannonsY = HEIGHT - 2 * TILE;
  for (int i = 0; i < cannonLives - 1; i++)
  {
    app->renderClip(3 * TILE + 16 * i, cannonsY, {0, 8, 16, 8});
  }
}
