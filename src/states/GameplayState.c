#include "StatesInternal.h"
#include "../Application.h"
#include "../Utils/Menu.h"
#include "../Utils/Timer.h"
#include "../Utils/Renderer.h"

// ========================================================================= //
// HORDE
// ========================================================================= //

#define INVADER_COUNT 55

#define INVADER_DEATH_TIMEOUT 400

struct Invader
{
    int x, y;
    bool dead;
};

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 7 * APP_FONT_PTSIZE

#define HORDE_MOVE_TIMEOUT_INIT 1000

struct Horde
{
    enum AtlasClip row0Clip, row12Clip, row34Clip;

    struct Invader invaders[INVADER_COUNT];

    bool moveRight;
    int moveCount;
    struct Timer moveTimer; // lower timeout -> faster invaders
} horde;

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
    horde.moveTimer = (struct Timer){
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
// GLOBALS
// ========================================================================= //

#define START_ANIMATION_STEP 20

bool multiplayer;

bool gamePaused;
bool startAnimation;
int startAnimationI, startAnimationJ;
int startAnimationSteps;
struct Timer startAnimationStepTimer;

Uint32 score1, score2, hiScore;
int score1X, score2X, hiScoreX;
int scoresY; // all scores share same y position

void InitGlobals()
{
    multiplayer = false;

    gamePaused = false;
    startAnimation = true;
    startAnimationSteps = 0;
    startAnimationI = 4;
    startAnimationJ = 0;
    startAnimationStepTimer = (struct Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = START_ANIMATION_STEP
    };

    score1 = score2 = hiScore = 0;

    score1X = 3 * APP_FONT_PTSIZE;
    score2X = APP_VSCREEN_WIDTH - 7 * APP_FONT_PTSIZE;
    hiScoreX = 12 * APP_FONT_PTSIZE;

    scoresY = 3 * APP_FONT_PTSIZE;
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

    UpdateTimer(&startAnimationStepTimer);
    if (startAnimationStepTimer.reachedTimeout)
    {
        horde.invaders[11 * startAnimationI + startAnimationJ].dead = false;

        if (startAnimationI == 0 && startAnimationJ == 10)
            startAnimation = false;
        else if (++startAnimationJ == 11)
        {
            startAnimationI--;
            startAnimationJ = 0;
        }        
    }
}

void RenderScores()
{
    char tmpScoreStr[5];

    SDL_snprintf(tmpScoreStr, 5, "%0*u", 4, score1);
    RenderText(
        score1X - 2 * APP_FONT_PTSIZE, scoresY - 2 * APP_FONT_PTSIZE, "SCORE<1>", false
    );
    RenderText(score1X, scoresY, tmpScoreStr, false);

    SDL_snprintf(tmpScoreStr, 5, "%0*u", 4, score2);
    RenderText(
        score2X - 2 * APP_FONT_PTSIZE, scoresY - 2 * APP_FONT_PTSIZE, "<2>SCORE", false
    );
    RenderText(score2X, scoresY, tmpScoreStr, false);

    SDL_snprintf(tmpScoreStr, 5, "%0*u", 4, hiScore);
    RenderText(
        hiScoreX - 2 * APP_FONT_PTSIZE, scoresY - 2 * APP_FONT_PTSIZE, "HI-SCORE", false
    );
    RenderText(hiScoreX, scoresY, tmpScoreStr, false);
}

// ========================================================================= //
// PLAYERS
// ========================================================================= //

struct Player
{
    int cannonX;

    int lives;
} p1, p2;


// ========================================================================= //
// MENUS
// ========================================================================= //

struct Menu pause;

void InitMenu()
{
    gamePaused = false;
}

void UpdateMenu()
{
    if (app.event.type == SDL_KEYDOWN && !app.event.key.repeat)
    {
        switch (app.event.key.keysym.sym)
        {
        case SDLK_RETURN:
            // do selected action
            break;
        case SDLK_UP:
            // go up
            break;
        case SDLK_DOWN:
            // go down
            break;
        }
    }
}

void RenderMenu()
{
    if (!gamePaused)
        return;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);

    // Semi-transparent background. Feels like a pause menu
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 100);
    SDL_RenderFillRect(app.renderer, NULL);


    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}


// ========================================================================= //
// GAMEPLAY STATE
// ========================================================================= //

void InitGameplayState()
{
    InitGlobals();
    InitMenu();
    InitHorde();
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
    {
        UpdateMenu();
        return;
    }

    MoveHorde();
}

void RenderGameplayState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    RenderScores();
    
    RenderHorde();
    RenderMenu();

    SDL_RenderPresent(app.renderer);
}