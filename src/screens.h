#pragma once

#include <functional>

#define MAX_SCREENS 8
#define SCREEN_EXIT_HOOK -1
#define SCREEN_UNCHANGED -2

struct Screen {
  std::function<void()> init, draw;
  std::function<int(const SDL_Event &)> update;
};

void splash_init();
void splash_draw();
int splash_update(const SDL_Event &event);
