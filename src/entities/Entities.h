#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL.h>
#include <stdbool.h>

#include "../util/Timer.h"

// ========================================================================= //
// Rendering Utils
// ========================================================================= //

// NOTE: there isn't ENTITYTEX_BUNKER because it's an unique entity.
// Take a look at atlas.png and you won't find a bunker image there.
enum EntityTex
{
    ENTITYTEX_TOURIST,
    ENTITYTEX_TOURIST_DEAD,

    ENTITYTEX_CANNON,
    ENTITYTEX_CANNON_DEAD0,
    ENTITYTEX_CANNON_DEAD1,

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

// 5 rows with 11 invaders each
#define INVADER_COUNT 55

#define INVADER_DEATH_TIMEOUT 400

struct Invader
{
    int x, y;

    bool dead;
    struct Timer deathTimer;
};

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 60

#define HORDE_STAND_TIMEOUT_INIT 1000

struct Horde
{
    enum EntityTex row0Tex, row12Tex, row34Tex;

    struct Invader invaders[INVADER_COUNT];
    int deadCount;

    int whoShot;

    bool moveRight;
    int moveCount;
    struct Timer moveTimer; // lower timeout -> faster invaders
};

void InitHorde(struct Horde* horde);
void UpdateHorde(struct Horde* horde);
void RenderHorde(const struct Horde* horde);


// ========================================================================= //
// Cannon
// ========================================================================= //

#define CANNON_X_INIT 26
#define CANNON_Y      220 // won't change

#define CANNON_SHOT_COOLDOWN 1500
#define CANNON_DEATH_TIMEOUT 2000
#define CANNON_DEATH_TEX_SWAP_TIMEOUT 100

struct Cannon
{
    enum EntityTex deathTex;

    int x;

    bool dead;
    int deaths;
    struct Timer deathTimer, deathTexSwapTimer;
    
    bool shooting;
    struct Timer shotTimer;
};

void InitCannon(struct Cannon* cannon);
void UpdateCannon(struct Cannon* cannon);
void RenderCannon(struct Cannon* cannon);


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
    struct BunkerPiece
    {
        int x, y;

        bool damaged;
    } pieces[352]; // 352 = 22 * 16 = w * h
};



#endif // ENTITIES_H
