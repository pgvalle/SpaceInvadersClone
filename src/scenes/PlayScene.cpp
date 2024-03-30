#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"


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
    marcelo.push_back(cannon->shoot());
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

    // shots update and collision checks
    for (int i = 0; i < marcelo.size(); i++)
    {
      Shot& shot = marcelo[i];
      const SDL_Rect shotRect = { shot.x, shot.y, 1, 8 };
      shot.update();

      // collision with horde
      Explosion e = horde.checkAndProcessHit(shotRect);
      if (!e.hasFinished()) // valid explosion. Collision occurred
      {
        marcelo.erase(marcelo.begin() + i--);
        // handle

        continue;
      }

      // collision with ufo
      if (ufo)
      {
        ufo->checkAndProcessHit(shotRect);
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
  for (Shot& shot : marcelo)
  {
    shot.render();
  }

  //ufo->render();
  horde.render();

  if (cannon)
  {
    cannon->render();
  }

  if (ufo)
  {
    ufo->render();
  }

  // render life counter
  char livesFmt[] = {'0' + cannonLives, '\0'};
  app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);

  // render cannons
  const int cannonsY = HEIGHT - 2 * TILE;
  for (int i = 0; i < cannonLives - 1; i++)
  {
    app->renderClip(3 * TILE + 16 * i, cannonsY, {0, 8, 16, 8});
  }
}
