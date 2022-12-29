#ifndef ENTITIES_H
#define ENTITIES_H

#include <stdbool.h>


// NOTE: there isn't ENTITYTEX_BUNKER because it's an unique entity.
// Take a look at atlas.png and you won't find a bunker image there.
enum EntityTex
{
    ENTITYTEX_TOURIST,
    ENTITYTEX_TOURIST_DEAD,

    ENTITYTEX_CANNON,
    ENTITYTEX_DEAD0,
    ENTITYTEX_DEAD1,

    ENTITYTEX_INVADER00,
    ENTITYTEX_INVADER01,
    ENTITYTEX_INVADER10,
    ENTITYTEX_INVADER11,
    ENTITYTEX_INVADER20,
    ENTITYTEX_INVADER21,

    ENTITYTEX_INVADER_DEAD,

    ENTITYTEX_COUNT
};

// x and y are in-game coordinates.
void RenderEntity(int x, int y, enum EntityTex tex);


// ========================================================================= //
// Horde
// ========================================================================= //

#define HORDE_SIZE 55

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 60

#define HORDE_STAND_TIMEOUT_INIT 1000

#define INVADER_DEATH_TIMEOUT 400

struct Horde
{
    int bruh;
};


// ========================================================================= //
// Cannon
// ========================================================================= //

#define CANNON_X_INIT 26
#define CANNON_Y      220 // won't change

#define CANNON_SHOT_COOLDOWN 1000

#define CANNON_DEATH_TIMEOUT 1000

#define CANNON_DEATHTEX_SWAP_TIMEOUT 100

struct Cannon
{
    int a;
};


// ========================================================================= //
// Tourist
// ========================================================================= //

struct Tourist
{
    int a;
};


// ========================================================================= //
// Bunker
// ========================================================================= //

struct Bunker
{
    SDL_Rect pieces[]
};

#endif // ENTITIES_H
