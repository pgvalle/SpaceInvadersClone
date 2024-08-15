#ifndef CORE_COMMON
#define CORE_COMMON

#include "util/timer.h"

#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_FontCache.h>

// No need to re-tell in a .c/.cpp what we declared in a .h as global (extern)
#ifndef DECL
#define DECL(type, name1, ...) extern type name1, ##__VA_ARGS__
#endif

#define TILE 8
#define WIDTH (30 * TILE)
#define HEIGHT (32 * TILE)

#define FPS 60

#endif // CORE_COMMON
