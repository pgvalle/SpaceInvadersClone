#ifndef GAMEPLAY_INTERNAL_H
#define GAMEPLAY_INTERNAL_H

#include <stdlib.h>
#include <stdbool.h>
#include "../globals.h"
#include "../../core.h"
#include "../../utils.h"

// ========================================================================= //
// GLOBAL STUFF
// ========================================================================= //

#define START_ANIMATION_STEP 10

// ========================================================================= //
// HORDE
// ========================================================================= //

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 9 * FONT_PTSIZE

#define HORDE_INVADER_COUNT 55

#define HORDE_MOVE_TIMEOUT_INIT 1024

struct Invader
{
    int x, y;
    bool dead;
};

extern struct Horde
{
    int clipIndexOffset; // what clip to use to render invaders
    struct Invader invaders[HORDE_INVADER_COUNT];
    bool moveRight;
    int moveCount;
    t1mer_t moveTimer; // lower timeout -> faster invaders
} horde;

void InitHorde();
void MoveHorde();
void RenderHorde();

// ========================================================================= //
// TOURIST
// ========================================================================= //

#define TOURIST_Y (HORDE_Y_INIT - 3 * FONT_PTSIZE)

#define TOURIST_VEL 0.65f

extern struct Tourist
{
    float x;
    bool moveRight;
    bool dead;
    bool spawned;
    t1mer_t spawnTimer;
} tourist;

void InitTourist();
void UpdateTourist();
void RenderTourist();

// ========================================================================= //
// CANNON
// ========================================================================= //

#define CANNON_X_INIT 14
#define CANNON_Y      (HORDE_Y_INIT + 19 * FONT_PTSIZE)

#define CANNON_VEL 2

#define CANNON_SHOT_COOLDOWN 512

extern struct Cannon
{
    int x;
    bool dead;
    int deaths;
    t1mer_t shotTimer;
} cannon;

void InitCannon();
void UpdateCannon();
void RenderCannon();

// ========================================================================= //
// EXPLOSIONS
// ========================================================================= //

#define EXPLOSION_INVADER_TIMEOUT 300
#define EXPLOSION_TOURIST_TIMEOUT 1000
#define EXPLOSION_SHOT_TIMEOUT    350

extern struct Explosion
{
    int x, y;
    animation_t animation;
}* explosions;

void AddExplosion(int x, int y, const struct animation_t* anim);
void UpdateExplosions();
void RenderExplosions();

// ========================================================================= //
// SHOTS
// ========================================================================= //

#define SHOT_CANNON_VEL 4

extern struct InvaderShot
{
    int x, y;
    animation_t animation; // each shot has its own animation state
}* invaderShots;

extern struct CannonShot
{
    int x, y;
}* cannonShots;

void InitShots();
void UpdateShots();
void RenderShots();

// ========================================================================= //
// Bunkers
// ========================================================================= //

#define BUNKER_FIRST_X 4 * FONT_PTSIZE
#define BUNKERS_Y CANNON_Y - 3 * FONT_PTSIZE

extern struct Bunker
{
    SDL_Point pieces[352]; // 22 * 16
} bunkers[4];

void InitBunkers();
void RenderBunkers();

// ========================================================================= //
// COLLISIONS
// ========================================================================= //

void ProcessTouristCollision();
void ProcessHordeCollisions();
void ProcessBunkersCollisions();
void ProcessCannonCollision();


#endif // GAMEPLAY_INTERNAL_H