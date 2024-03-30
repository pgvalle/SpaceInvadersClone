#pragma once


#define TITLE "Space Invaders Clone"

#define FPS 60
#define FRAMERATE (1000 / FPS)

#ifndef ASSETS_DIR
#define ASSETS_DIR "./res/"
#endif

#define TILE 8
// These are the dimensions of the canvas. NOT of the window.
#define WIDTH  (28 * TILE) // 224
#define HEIGHT (32 * TILE) // 240
