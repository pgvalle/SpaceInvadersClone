#include "PlayScene.h"
#include "SIC.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::processEvent(const SDL_Event &event)
{
  if (event.type == SDL_QUIT)
    sic->shouldStop = true;

  /*
  // check input
  shotCooldown.update(dt);
  if (shotCooldown.hasTimedOut() && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE])
  {
    shots.push_back(cannon.shoot());
    shotCooldown.reset();
  }

  cannon.onUpdate(dt);
  ufo.onUpdate(dt);
  horde.onUpdate(dt);

  for (int i = 0; i < explosions.size(); i++)
  {
    Explosion *exp = explosions[i];
    exp->onUpdate(dt);
    if (exp->lifespan.hasTimedOut())
    {
      explosions.erase(explosions.begin() + i--);
      delete exp;
    }
  }

  for (int i = 0; i < shots.size(); i++)
  {
    Shot *shot = shots[i];
    shot->onUpdate(dt);
    // TODO: check collision with player, ufo and invaders
  }
  */
}

void PlayScene::tick(float dt)
{
}

void PlayScene::draw() const
{
  /*
  cannon.onRender();
  ufo.onRender();
  horde.onRender();

  for (Shot *shot : shots)
    shot->onRender();

  for (Explosion *exp : explosions)
    exp->onRender();

  for (const Bunker &bun : bunkers)
    bun.onRender();
  */
}
