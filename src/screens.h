#pragma once

#include <functional>

#define NULL_SCREEN -1

struct Screen {
  std::function<void()> init, draw;
  std::function<int(const SDL_Event &)> update;
};

#define SPLASH_SCREEN 0

void splash_init();
void splash_draw();
int splash_update(const SDL_Event &event);

#define READY_SCREEN 1

void ready_init();
void ready_draw();
int ready_update(const SDL_Event &event);

#define PLAY_SCREEN 2

void play_init();
void play_draw();
int play_update(const SDL_Event &event);
