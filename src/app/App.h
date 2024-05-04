#pragma once

#include "scenes/Scene.h"

#include <SDL.h>

// no need to define variables in a implementation file again
#ifndef EXT
#define EXT extern
#endif

void run();

// so that scenes can close app
EXT bool shouldClose;

// utils

void closeApp();

bool isKeyPressed(int key);

void renderText(int x, int y, const char *text,
                SDL_Color color = {255, 255, 255, 255});
void renderClip(int x, int y, const SDL_Rect &clip);


#ifdef APP_INTERNALS

EXT SDL_Window* window;
EXT SDL_Renderer *renderer;

// assets
EXT SDL_Texture *atlas, *texAtlas;

#endif
