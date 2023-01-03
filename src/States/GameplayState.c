#include "StatesInternal.h"
#include "../Application.h"
#include "../Utils/Menu.h"
#include "../Utils/Animation.h"
#include "../Utils/Renderer.h"
#include "../Utils/stb_ds.h"

#include <string.h>

/*****************************************************************************/
/* DECLARATIONS */
/*****************************************************************************/

// ========================================================================= //
// GLOBALS
// ========================================================================= //

#define START_ANIMATION_STEP 10

bool gamePaused;
bool startAnimation;
int startAnimationSteps;

void InitGlobals();
void UpdateGlobals();


// ========================================================================= //
// HORDE
// ========================================================================= //

#define INVADER_COUNT 55

struct Invader
{
    int x, y;
    bool dead;
};

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 72

#define HORDE_MOVE_TIMEOUT_INIT 1024

struct Horde
{
    int clipIndexOffset; // what clip to use to render invaders

    struct Invader invaders[INVADER_COUNT];

    bool moveRight;
    int moveCount;
    Timer moveTimer; // lower timeout -> faster invaders
} horde;

void InitHorde();
void MoveHorde();
void RenderHorde();


// ========================================================================= //
// TOURIST
// ========================================================================= //

#define TOURIST_Y HORDE_Y_INIT - 3 * APP_FONT_PTSIZE

#define TOURIST_VEL 0.75f

struct Tourist
{
    float x;
    bool moveRight;

    bool dead;

    bool spawned;
    Timer spawnTimer;
} tourist;

void InitTourist();
void UpdateTourist();
void RenderTourist();


// ========================================================================= //
// CANNONS
// ========================================================================= //

#define CANNON_Y 20 * APP_FONT_PTSIZE

#define CANNON_VEL 1

struct Cannon
{
    int x;

    bool dead;
} p1Cannon, p2Cannon;

void InitCannons();


// ========================================================================= //
// EXPLOSIONS
// ========================================================================= //

#define EXPLOSION_INVADER_TIMEOUT 300
#define EXPLOSION_TOURIST_TIMEOUT 1000
#define EXPLOSION_SHOT_TIMEOUT    350

struct Explosion
{
    int x, y;
    struct Animation animation;
}* explosions;

void AddExplosion(int x, int y, const struct Animation* anim);
void UpdateExplosions();
void RenderExplosions();


// ========================================================================= //
// SHOTS
// ========================================================================= //

struct InvaderShot
{
    int x, y;
    struct Animation animation; // each shot has its own animation state
}* invadersShots;
int invaderShotsVel;

#define CANNON_SHOT_VEL 4

struct CannonShot
{
    int x, y;
}* cannonShots;

void InitShots();
void UpdateShots();
void RenderShots();


/*****************************************************************************/
/* IMPLEMENTATIONS */
/*****************************************************************************/

// ========================================================================= //
// GLOBALS
// ========================================================================= //

void InitGlobals()
{
    gamePaused = false;
    startAnimation = true;
    startAnimationSteps = 0;
}

void UpdateGlobals()
{
    if (app.event.type == SDL_KEYDOWN && !app.event.key.repeat)
    {
        if (app.event.key.keysym.sym == SDLK_ESCAPE)
            gamePaused = !gamePaused;
    }

    if (!startAnimation || gamePaused)
        return;

    // Only executes if the game isn't paused and start animation is playing

    // "undeading" invaders starting from bottom-left
    const int i = 4 - startAnimationSteps / 11;
    const int j = startAnimationSteps % 11;
    horde.invaders[11 * i + j].dead = false;

    // updating animation status
    if (++startAnimationSteps == INVADER_COUNT)
        startAnimation = false;
}


// ========================================================================= //
// HORDE
// ========================================================================= //

void InitHorde()
{
    horde.clipIndexOffset = 1;

    for (int i = 0; i < 11; i++)
    {
        // first row
        horde.invaders[i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[i].y = HORDE_Y_INIT;
        // second
        horde.invaders[11 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[11 + i].y = HORDE_Y_INIT + 16;
        // third
        horde.invaders[22 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[22 + i].y = HORDE_Y_INIT + 32;
        // fourth
        horde.invaders[33 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[33 + i].y = HORDE_Y_INIT + 48;
        // fifth
        horde.invaders[44 + i].x = HORDE_X_INIT + 16 * i;
        horde.invaders[44 + i].y = HORDE_Y_INIT + 64;
    }

    // All invaders start dead to play an animation before starting gameplay
    for (int i = 0; i < INVADER_COUNT; i++)
        horde.invaders[i].dead = true;

    horde.moveRight = true;
    horde.moveCount = 8;
    horde.moveTimer = (Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = HORDE_MOVE_TIMEOUT_INIT
    };
}

bool HasHordeReachedBounds()
{
    for (int i = 0; i < INVADER_COUNT; i++)
    {
        if (horde.invaders[i].dead)
            continue;

        const int x = horde.invaders[i].x;
        if (x <= 10 || x >= APP_VSCREEN_WIDTH - 22)
            return true;
    }

    return false;
}

void MoveHorde()
{
    UpdateTimer(&horde.moveTimer);
    // can't move yet
    if (!horde.moveTimer.reachedTimeout)
        return;

    // should move down
    if (HasHordeReachedBounds())
    {
        for (int i = 0; i < INVADER_COUNT; i++)
            horde.invaders[i].y += 8;

        horde.moveCount = 0; // reset move counter
        if (horde.moveTimer.timeout >= 16)
            horde.moveTimer.timeout -= 16;
        horde.moveRight = !horde.moveRight; // change horizontal movement
    }
    
    const int offset = horde.moveRight ? 2 : -2;
    for (int i = 0; i < INVADER_COUNT; i++)
        horde.invaders[i].x += offset;

    horde.moveCount++;
    horde.clipIndexOffset = !horde.clipIndexOffset;
}

void RenderHorde()
{
    for (int i = 0; i < INVADER_COUNT; i++)
    {
        if (horde.invaders[i].dead)
            continue;

        const int x = horde.invaders[i].x;
        const int y = horde.invaders[i].y;

        switch (i / 11)
        {
        case 0: // first row
            RenderAtlasClip(x, y, ATLASCLIP_INVADER00 + horde.clipIndexOffset);
            break;
        case 1: case 2: // second or third
            RenderAtlasClip(x, y, ATLASCLIP_INVADER10 + horde.clipIndexOffset);
            break;
        case 3: case 4: // fourth or fifth
            RenderAtlasClip(x, y, ATLASCLIP_INVADER20 + horde.clipIndexOffset);
            break;
        }
    }
}


// ========================================================================= //
// TOURIST
// ========================================================================= //

void InitTourist()
{
    tourist.x = 0;
    tourist.moveRight = false;

    tourist.dead = false;

    tourist.spawned = false;
    tourist.spawnTimer = (Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = 5000
    };
}

void UpdateTourist()
{
    if (tourist.dead && tourist.spawned) // spawned and dead
    {
        tourist.x = 1000;
        tourist.dead = false; // not dead anymore. Didn't even spawn yet

        // reset spawn status
        tourist.spawned = false;
        tourist.spawnTimer = (Timer){
            .reachedTimeout = false,
            .time = 0,
            .timeout = rand() % 20000 + 10000
        };
    }
    else if (tourist.spawned) // spawned and not dead yet
    {
        const float offset = tourist.moveRight ? TOURIST_VEL : -TOURIST_VEL;
        tourist.x += offset;

        if (tourist.x <= 0.0f || tourist.x >= APP_VSCREEN_WIDTH - 22.f)
        {
            // reset spawn status
            tourist.spawned = false;
            tourist.spawnTimer = (Timer){
                .reachedTimeout = false,
                .time = 0,
                .timeout = rand() % 20000 + 10000
            };
        }
    }
    else // neither spawned nor dead
    {
        UpdateTimer(&tourist.spawnTimer);
        if (tourist.spawnTimer.reachedTimeout) // spawn
        {
            tourist.moveRight = rand() % 2;
            tourist.x = tourist.moveRight ? 0 : (APP_VSCREEN_WIDTH - 22.f);
            tourist.spawned = true;
        }
    }
}

void RenderTourist()
{
    if (!tourist.dead && tourist.spawned)
        RenderAtlasClip(roundf(tourist.x), TOURIST_Y, ATLASCLIP_TOURIST);
}


// ========================================================================= //
// EXPLOSIONS
// ========================================================================= //

void AddExplosion(int x, int y, const struct Animation* anim)
{
    const struct Explosion explosion = { .x = x, .y = y, .animation = *anim };
    arrput(explosions, explosion);
}

void UpdateExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        UpdateAnimation(&explosions[i].animation);
        if (HasAnimationFinished(&explosions[i].animation))
        {
            FreeAnimation(&explosions[i].animation);
            arrdel(explosions, i);
        }
    }
}

void RenderExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        RenderAnimation(
            explosions[i].x,
            explosions[i].y,
            &explosions[i].animation
        );
    }
}


// ========================================================================= //
// SHOTS
// ========================================================================= //

void InitShots()
{
    invaderShotsVel = 2;
}

void UpdateShots()
{
    for (int i = 0; i < arrlen(invadersShots); i++)
    {
        UpdateAnimation(&invadersShots[i].animation);
        if (HasAnimationFinished(&invadersShots[i].animation))
            ResetAnimation(&invadersShots[i].animation);

        invadersShots[i].y += invaderShotsVel;
    }

    for (int i = 0; i < arrlen(cannonShots); i++)
        cannonShots[i].y -= CANNON_SHOT_VEL;
}

void RenderShots()
{
    for (int i = 0; i < arrlen(invadersShots); i++)
    {
        RenderAnimation(
            invadersShots[i].x,
            invadersShots[i].y,
            &invadersShots[i].animation
        );
    }

    for (int i = 0; i < arrlen(cannonShots); i++)
        RenderAtlasClip(cannonShots[i].x, cannonShots[i].y, ATLASCLIP_CANNON_SHOT);
}

// ========================================================================= //
// COLLISIONS
// ========================================================================= //

void ProcessTouristCollision()
{
    const SDL_Rect touristRect = { roundf(tourist.x) + 4, TOURIST_Y, 16, 8};

    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        const SDL_Rect shotRect = { cannonShots[i].x, cannonShots[i].y, 1, 4 };
        if (SDL_HasIntersection(&shotRect, &touristRect))
        {
            Animation animation;
            InitAnimation(&animation, 1, (AnimationFrame){
                .clip = ATLASCLIP_TOURIST_EXPLOSION,
                    .timer = {
                    .reachedTimeout = false,
                    .time = 0,
                    .timeout = EXPLOSION_TOURIST_TIMEOUT
                }
            });
            AddExplosion(tourist.x, TOURIST_Y, &animation);

            tourist.dead = true;

            // remove shot from list
            arrdel(cannonShots, i);
        }
    }
}

void ProcessHordeCollisions()
{
    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        const SDL_Rect shotRect = { cannonShots[i].x, cannonShots[i].y, 1, 4 };
    
        // horde collisions
        for (int j = 0; j < INVADER_COUNT; j++)
        {
            if (horde.invaders[j].dead)
                continue;

            const int invaderX = horde.invaders[j].x;
            const int invaderY = horde.invaders[j].y;
            SDL_Rect invaderRect = { invaderX, invaderY, 12, 8 };
            if (j / 11 == 0)
                invaderRect = (SDL_Rect){ invaderX + 2, invaderY, 8, 8 };
            else if (j / 11 <= 2)
                invaderRect = (SDL_Rect){ invaderX + 1, invaderY, 11, 8 };

            if (SDL_HasIntersection(&shotRect, &invaderRect))
            {
                // create explosion at invaderX, invaderY
                Animation animation;
                InitAnimation(&animation, 1, (AnimationFrame){
                    .clip = ATLASCLIP_INVADER_EXPLOSION,
                    .timer = {
                        .reachedTimeout = false,
                        .time = 0,
                        .timeout = EXPLOSION_INVADER_TIMEOUT
                    }
                });
                AddExplosion(invaderX, invaderY, &animation);

                horde.invaders[j].dead = true;
                // increate invader speed
                if (horde.moveTimer.timeout >= 16)
                    horde.moveTimer.timeout -= 16;

                // This creates that delay effect in horde movement when an invader gets shot
                if (horde.moveTimer.timeout < horde.moveTimer.time + EXPLOSION_INVADER_TIMEOUT)
                {
                    if (horde.moveTimer.time >= EXPLOSION_INVADER_TIMEOUT)
                        horde.moveTimer.time -= EXPLOSION_INVADER_TIMEOUT;
                    else
                        horde.moveTimer.time = 0;
                }

                // remove shot from list
                arrdel(cannonShots, i);
            }
        }
    }
}

void ProcessBunkersCollisions()
{

}

void ProcessPlayerCollision()
{
    
}


// ========================================================================= //
// GAMEPLAY STATE
// ========================================================================= //

void InitGameplayState()
{
    InitGlobals();
    InitHorde();
    InitTourist();
}

void DestroyGameplayState()
{
    
}

void UpdateGameplayState()
{
    UpdateGlobals();
    if (startAnimation)
        return;

    if (gamePaused)
        return;

    if (app.event.type == SDL_KEYDOWN && app.event.key.keysym.sym == SDLK_SPACE)
    {
        struct CannonShot shot = {
            .x = rand() % 180 + 30,
            .y = 240
        };
        arrput(cannonShots, shot);
    }

    UpdateShots();
    ProcessHordeCollisions();
    ProcessTouristCollision();
    UpdateExplosions();
    MoveHorde();
    UpdateTourist();
}

void RenderGameplayState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
    
    RenderShots();
    RenderHorde();
    RenderTourist();
    RenderExplosions();

    RenderText(8, 168 + HORDE_Y_INIT, "3", false);

    SDL_RenderPresent(app.renderer);
}