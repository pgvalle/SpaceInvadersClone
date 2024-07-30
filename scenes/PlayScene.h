#ifndef _PLAYSCENE_
#define _PLAYSCENE_

#include "GameScene.h"

#include "entities/UFO.h"
#include "entities/Bunker.h"

class PlayScene : public GameScene
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

  UFO ufo;

  Bunker bunkers[4];

public:
  PlayScene(UIScene *ui);
  ~PlayScene();

  void processEvent(const SDL_Event &event) override;
  void update(float delta) override;
  void render(SDL_Renderer *renderer) override;
};

#endif /* _PLAYSCENE_ */
