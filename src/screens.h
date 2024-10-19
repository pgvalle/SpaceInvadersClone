#pragma once

#include <functional>

#define SCREEN_EXIT_HOOK -1
#define SCREEN_UNCHANGED -2

struct Screen {
  std::function<void()> init, draw;
  std::function<int(const SDL_Event &)> update;
};

void splash_init();
void splash_draw();
int splash_update(const SDL_Event &event);

void ready_init();
void ready_draw();
int ready_update(const SDL_Event &event);

void play_init();
void play_draw();
int play_update(const SDL_Event &event);
