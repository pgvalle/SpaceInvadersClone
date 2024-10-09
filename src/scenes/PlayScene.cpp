#include "PlayScene.h"
#include "SIC.h"

#include "entts/Entity.h"
#include "entts/UFO.h"
#include "entts/Horde.h"
#include "entts/Explosion.h"

PlayScene::PlayScene()
{
  entities.insert(entities.end(), {
    new UFO, new Horde
  });
}

PlayScene::~PlayScene()
{
  for (Entity *entt : entities)
    delete entt;

  for (Explosion *exp : explosions)
    delete exp;
}

void PlayScene::onTick(float dt)
{
  for (int i = 0; i < entities.size(); i++)
  {
    for (int j = i + 1; j < entities.size(); j++)
    {
      const SDL_Rect ri = entities[i]->getHitbox();
      const SDL_Rect rj = entities[j]->getHitbox();
      
      Explosion *ei = entities[i]->onHit(rj);
      Explosion *ej = entities[j]->onHit(ri);
      if (ei && ej)
        explosions.insert(explosions.end(), {ei, ej});
    }
  }
}

void PlayScene::onUpdate(float dt)
{
  for (Entity *entt : entities)
    entt->onUpdate(dt);

  for (int i = 0; i < explosions.size(); i++)
  {
    Explosion *e = explosions[i];
    e->onUpdate(dt);
    if (e->lifespan.hasTimedOut())
    {
      explosions.erase(explosions.begin() + --i);
      delete e;
    }
  }
}

void PlayScene::onRender() const
{
  for (Entity *entt : entities)
    entt->onRender();

  for (Explosion *exp : explosions)
    exp->onRender();
}
