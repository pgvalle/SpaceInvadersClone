#ifndef _GAMESCENE_
#define _GAMESCENE_

#include "UIScene.h"

class GameScene : public NAGE::Scene
{
protected:
  UIScene *ui;

public:
  GameScene(UIScene *a_ui) : ui(a_ui) {}
};

#endif /* _GAMESCENE_ */
