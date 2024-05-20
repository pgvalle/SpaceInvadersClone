#include "PlayScene.h"
#include "Events.h"

#include "App.h"

PlayScene::PlayScene(UIScene *ui) : GameScene(ui)
{
}

PlayScene::~PlayScene()
{
}


void PlayScene::processEvent(const SDL_Event &event)
{
  ui->processEvent(event);
}

void PlayScene::update(float delta)
{
  ui->update(delta);
  switch (state)
  {
  case STARTING:
    // horde.update();

    // delayer.update();
    // if (delayer.hasTimedOut())
    // {
    //   state = PLAYING;
    //   cannon = new Cannon();
    // }

    break; 
  case PLAYING:
    // update these first
    // updateExplosions();
    // updateShotsAndCollisions();

    // updatePlayerShooting();
    // ufo.update();

    // horde.update();

    // cannon->update();
    // if (cannon->checkAndProcessHit({WIDTH - 5 * TILE, HEIGHT - 5 * TILE, 20, 8}))
    // {
    //   state = DELAYING;
    // }

    break;
  case DELAYING:
    // if (cannon->isDead())
    // {
    //   delayer.update();
    //   if (!delayer.hasTimedOut()) break;
      
    //   state = PLAYING;
    //   delete cannon;
    //   cannon = new Cannon();      
    // }
    // else
    // {
    //   // not dead but may be dead after update
    //   cannon->update();
    //   if (!cannon->isDead()) break;

    //   delayer.reset(2000); // wait 3 seconds after cannon death
    //   cannonLives--;
    // }

    break;
  default:
    break;
  }
}

void PlayScene::render(SDL_Renderer *renderer)
{
  ui->render(renderer);

  for (int i = 0; i < 4; i++)
  {
    bunkers[i].render(renderer);
  }
}