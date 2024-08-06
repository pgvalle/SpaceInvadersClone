#ifndef SCENES_GAME_GAME
#define SCENES_GAME_GAME

#include <common.h>

class Game : public Scene {
public:
  Game();
  ~Game();

  void process_event(Engine *eng, const SDL_Event &evt);
  void update(Engine *eng);
  void render(const Engine *eng);
};

#endif  // SCENES_GAME_GAME
