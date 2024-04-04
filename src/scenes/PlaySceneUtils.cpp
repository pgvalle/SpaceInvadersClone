#include "app/App.h"
#include "PlayScene.h"
#include "defines.h"


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

void PlayScene::updateExplosions()
{
  for (size_t i = 0; i < explosions.size(); i++)
  {
    explosions[i].update();
    if (explosions[i].hasFinished())
    {
      explosions.erase(explosions.begin() + i--);
    }
  }
}

void PlayScene::updateShotsAndCollisions()
{
  // shots (they generate explosions)
  for (size_t i = 0; i < shots.size(); i++)
  {
    // get up-to-date shot hitbox
    shots[i].update();
    const SDL_Rect shotHB = {shots[i].x, shots[i].y, 1, 8};

    // collision with horde
    {
      Explosion e = horde.checkAndProcessHit(shotHB);
      if (!e.hasFinished()) // Valid explosion. Collision.
      {
        explosions.push_back(e);
        shots.erase(shots.begin() + i--);
        continue;
      }
    }

    // collision with ufo
    if (ufo.checkAndProcessHit(shotHB))
    {
      shots.erase(shots.begin() + i--);
      continue;
    }

    // collision with player
    // if (cannon && cannon->checkAndProcessHit(shotHB))
    // {
    //   shots.erase(shots.begin() + i--);
    //   // explosion here as well
    //   continue;
    // }

    // collision with top of world
    if (shotHB.y <= 4 * TILE)
    {
      explosions.push_back({shotHB.x - 3, 4 * TILE, 200, {36, 24, 8, 8}});
      shots.erase(shots.begin() + i--);
      continue;
    }

    // with bottom of world
    //...

    // collision with other shots
    for (size_t j = i + 1; j < shots.size(); j++)
    {
      const SDL_Rect shot2HB = { shots[j].x, shots[j].y, 1, 8 };
      if (SDL_HasIntersection(&shotHB, &shot2HB))
      {
        // erase both shots and create explosion accordingly
        // shot1
        explosions.push_back({shotHB.x - 3, shotHB.y, 200, {36, 24, 8, 8}})
        shots.erase(shots.begin() + i--);
        // shot2
        explosions.push_back({shot2HB.x - 3, shot2HB.y, 200, {36, 24, 8, 8}})
        shots.erase(shots.begin() + j - 1);
        break;
      }
    }
  }
}
