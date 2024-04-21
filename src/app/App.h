#pragma once

#include "scenes/Scene.h"

#include <SDL.h>
#include <SDL_ttf.h>


void run();

// globals

extern SDL_Renderer *renderer;

extern int score, hiScore, coins;

extern SDL_Event event;
extern Uint32 dt;

// utils

// passing nullptr will close game
// calling twice in same frame is safe. Ignore previous calls.
void changeScene(Scene* newScene);

bool isKeyPressed(int key);

void renderText(int x, int y, const char *text,
                SDL_Color color = {255, 255, 255, 255});
void renderClip(int x, int y, const SDL_Rect &clip);


#ifdef APP_INTERNALS

extern SDL_Window* window;

// assets
extern SDL_Texture *atlas;
extern TTF_Font *font;

// to change scene inside a scene
extern Scene *scene, *nextScene;
extern bool sceneChange;

void mainLoop();

#endif
