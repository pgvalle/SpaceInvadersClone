#include "StatesInternal.h"
#include "../Application.h"
#include "../Utils/Menu.h"
#include "../Utils/ClipAnimation.h"
#include "../Utils/Renderer.h"
#include "../Utils/stb_ds.h"

#include <string.h>

/*****************************************************************************/
/* DECLARATIONS */
/*****************************************************************************/

// ========================================================================= //
// GLOBALS
// ========================================================================= //

#define START_ANIMATION_STEP 20

bool gamePaused;
bool startAnimation;
int startAnimationSteps;
Timer startAnimationStepTimer;

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
#define HORDE_Y_INIT 9 * APP_FONT_PTSIZE

#define HORDE_MOVE_TIMEOUT_INIT 1000

struct Horde
{
    enum AtlasClip row0Clip, row12Clip, row34Clip;

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
// EXPLOSIONS
// ========================================================================= //

#define EXPLOSION_INVADER_DEATH_TIMEOUT 350
#define EXPLOSION_TOURIST_DEATH_TIMEOUT 1000
#define EXPLOSION_SHOT_DEATH_TIMEOUT    350

struct Explosion
{
    int x, y;
    struct ClipAnimation animation;
}* explosions;

void UpdateExplosions();
void RenderExplosions();


// ========================================================================= //
// SHOTS
// ========================================================================= //

struct Shots
{
    int x, y;
    struct ClipAnimation animation;
}* shots;

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
    startAnimationStepTimer = (Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = START_ANIMATION_STEP
    };
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

    // Only executes if the game isn't paused and start animation is being played

    UpdateTimer(&startAnimationStepTimer);
    if (startAnimationStepTimer.reachedTimeout)
    {
        // "undeading" invaders
        const int i = 4 - startAnimationSteps / 11;
        const int j = startAnimationSteps % 11;
        horde.invaders[11 * i + j].dead = false;

        // updating animation status
        if (++startAnimationSteps == INVADER_COUNT)
            startAnimation = false;
    }
}


// ========================================================================= //
// HORDE
// ========================================================================= //

void InitHorde()
{
    horde.row0Clip  = ATLASCLIP_INVADER00;
    horde.row12Clip = ATLASCLIP_INVADER10;
    horde.row34Clip = ATLASCLIP_INVADER20;

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

void MoveHorde()
{
    UpdateTimer(&horde.moveTimer);
    // can't move yet
    if (!horde.moveTimer.reachedTimeout)
        return;

    // should move down
    if (horde.moveCount == 16)
    {
        for (int i = 0; i < INVADER_COUNT; i++)
            horde.invaders[i].y += 8;

        horde.moveCount = 0; // reset move counter
        horde.moveTimer.timeout; // increase invaders speed somehow
        horde.moveRight = !horde.moveRight; // change horizontal movement
    }
    else
    {
        int offset = horde.moveRight ? 2 : -2;
        for (int i = 0; i < INVADER_COUNT; i++)
            horde.invaders[i].x += offset;

        horde.moveCount++;
    }

    // change animation frame for all invaders in horde
    if (horde.row0Clip  == ATLASCLIP_INVADER00)
    {
        horde.row0Clip  = ATLASCLIP_INVADER01;
        horde.row12Clip = ATLASCLIP_INVADER11;
        horde.row34Clip = ATLASCLIP_INVADER21;
    }
    else
    {
        horde.row0Clip  = ATLASCLIP_INVADER00;
        horde.row12Clip = ATLASCLIP_INVADER10;
        horde.row34Clip = ATLASCLIP_INVADER20;
    }
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
            RenderAtlasClip(x, y, horde.row0Clip);
            break;
        case 1: case 2: // second or third
            RenderAtlasClip(x, y, horde.row12Clip);
            break;
        case 3: case 4: // fourth or fifth
            RenderAtlasClip(x, y, horde.row34Clip);
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
        float offset = tourist.moveRight ? 0.65f : -0.65f;
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
            tourist.x = tourist.moveRight ? 0 : (APP_VSCREEN_WIDTH - 22);
            tourist.spawned = true;
        }
    }
}

void RenderTourist()
{
    if (!tourist.dead && tourist.spawned)
        RenderAtlasClip((int)roundf(tourist.x), TOURIST_Y, ATLASCLIP_TOURIST);
}


// ========================================================================= //
// EXPLOSIONS
// ========================================================================= //

void UpdateExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        UpdateClipAnimation(&explosions[i].animation);
        if (HasClipAnimationFinished(&explosions[i].animation))
        {
            FreeClipAnimation(&explosions[i].animation);
            arrdel(explosions, i);
        }
    }
}

void RenderExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        RenderClipAnimation(
            explosions[i].x,
            explosions[i].y,
            &explosions[i].animation
        );
    }
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
        int i = rand() % INVADER_COUNT;
        /*if (!horde.invaders[i].dead)
        {
            const struct Explosion explosion = {
                .x = horde.invaders[i].x,
                .y = horde.invaders[i].y
            };
            InitClipAnimation(&explosion.animation, 1, (struct ClipAnimationFrame){
                .clip = ATLASCLIP_INVADER_DEAD,
                .timer = {
                    .reachedTimeout = false,
                    .time = 0,
                    .timeout = EXPLOSION_INVADER_DEATH_TIMEOUT
                }
            });
            arrput(explosions, explosion);
        }*/
        
        if (!horde.invaders[i].dead)
        {
            const struct Explosion explosion = {
                .x = horde.invaders[i].x,
                .y = horde.invaders[i].y
            };
            InitClipAnimation(&explosion.animation, 1, (ClipAnimationFrame){
                .clip = ATLASCLIP_INVADER_DEAD,
                .timer = {
                    .reachedTimeout = false,
                    .time = 0,
                    .timeout = EXPLOSION_INVADER_DEATH_TIMEOUT
                }
            });
            arrput(explosions, explosion);
        }
        horde.invaders[i].dead = true;
    }

    UpdateExplosions();
    MoveHorde();
    UpdateTourist();
}

void RenderGameplayState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
    
    RenderHorde();
    RenderTourist();
    RenderExplosions();

    SDL_RenderPresent(app.renderer);
}