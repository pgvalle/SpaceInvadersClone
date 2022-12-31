#ifndef ENTITIES_H
#define ENTITIES_H

#include <SDL.h>
#include <stdbool.h>

#include "../Utils/Timer.h"


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

struct BunkerPiece
{
    int x, y;
    bool damaged;
};

#define BUNKER_COUNT 4

#define BUNKER0_X 
#define BUNKER0_Y

#define BUNKER1_X
#define BUNKER1_Y

#define BUNKER2_X
#define BUNKER2_Y

#define BUNKER3_X
#define BUNKER3_Y

struct Bunker
{
    struct BunkerPiece pieces[352]; // 352 = 22 * 16 = w * h
};

void InitBunkers(struct Bunker bunkers[]);
void UpdateBunkers(struct Bunker bunkers[]);
void RenderBunkers(struct Bunker bunkers[]);


#endif // ENTITIES_H
