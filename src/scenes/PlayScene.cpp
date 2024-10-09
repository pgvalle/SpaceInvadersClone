#include "PlayScene.h"
#include "SIC.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
  for (Explosion *exp : explosions)
    delete exp;
}

void PlayScene::onTick(float dt)
{
}

void PlayScene::onUpdate(float dt)
{
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
}

void PlayScene::onRender() const
{
  ufo.onRender();
  horde.onRender();

  for (Explosion *exp : explosions)
    exp->onRender();

  for (const Bunker &bun : bunkers)
    bun.onRender();
}
