#pragma once

#include "scenes/Scene.h"

#include <SDL.h>
#include <SDL_ttf.h>

// no need to declare everything in a cpp file again
#ifndef EXT
#define EXT extern
#endif

void run();

// globals

EXT int score, hiScore, coins;

// utils

// passing nullptr will close game
// calling twice in same frame is safe. Ignore previous calls.
void changeScene(Scene* newScene);

bool isKeyPressed(int key);

void renderText(int x, int y, const char *text,
                SDL_Color color = {255, 255, 255, 255});
void renderClip(int x, int y, const SDL_Rect &clip);


#ifdef APP_INTERNALS

EXT SDL_Window* window;
EXT SDL_Renderer *renderer;

// assets
EXT SDL_Texture *atlas;
EXT TTF_Font *font;

// to change scene inside a scene
EXT Scene *scene, *nextScene;
EXT bool sceneChange;

void mainLoop();

#endif
