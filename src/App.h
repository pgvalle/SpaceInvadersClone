#pragma once

#include "scenes/Scene.h"

#include <SDL.h>

/*
 * Useful aliases
 */

#define FPS 60
#define FRAME_DELAY (1000 / FPS)

#ifndef ASSETS_DIR
#define ASSETS_DIR "./res/"
#endif

#define TILE 8

// These are the dimensions of the canvas. NOT of the window.
#define WIDTH  (28 * TILE) // 224
#define HEIGHT (32 * TILE) // 240

// no need to redeclare variables in App.cpp file again
#ifndef EXTERN
#define EXTERN extern
#endif

/*
 * API
 */

// scene is owned by app
void run(Scene *scene);

// so that scenes can close app
EXTERN bool shouldClose;

bool isKeyPressed(int key);

void renderText(int x, int y, const char *text,
                SDL_Color color = {255, 255, 255, 255});
void renderClip(int x, int y, const SDL_Rect &clip);

#undef EXTERN

