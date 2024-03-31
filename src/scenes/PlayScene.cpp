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
  cannonLives = 3;
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
      shots[i].update();
      const SDL_Rect shotRectA = {shots[i].x, shots[i].y, 1, 8};

      // collision with horde
      Explosion explosion = horde.checkAndProcessHit(shotRectA);
      explosions.push_back(explosion);
      if (!explosion.hasFinished()) // valid explosion. Collision occurred
      {
        shots.erase(shots.begin() + i--);
        continue;
      }

      // collision with ufo
      if (ufo && ufo->checkAndProcessHit(shotRectA))
      {
        shots.erase(shots.begin() + i--);
        continue;
      }

      // collision with player
      // if (cannon && cannon->checkAndProcessHit(shotRectA))
      // {
      //   shots.erase(shots.begin() + i--);
      //   // explosion here as well
      //   continue;
      // }

      // collision with top or bottom of world
      if (shotRectA.y <= 4 * TILE)
      {
        shots.erase(shots.begin() + i--);
        Explosion e(shotRectA.x - 3, 4 * TILE, 200, {36, 24, 8, 8});
        explosions.push_back(e);
        continue;
      }

      // collision with other shots
      for (size_t j = i + 1; j < shots.size(); j++)
      {
        const SDL_Rect shotRectB = { shots[j].x, shots[j].y, 1, 8 };
        if (SDL_HasIntersection(&shotRectA, &shotRectB))
        {
          shots.erase(shots.begin() + i--);
          shots.erase(shots.begin() + j - 1);
          // explosion for each shot
          break;
        }
      }
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

  const int cannonsY = HEIGHT - 2 * TILE;
  for (int i = 0; i < cannonLives - 1; i++)
  {
    app->renderClip(3 * TILE + 16 * i, cannonsY, {0, 8, 16, 8});
  }

  char livesFmt[2];
  livesFmt[0] = (char)cannonLives + '0';
  app->renderText(TILE, HEIGHT - 2 * TILE, livesFmt);
}
