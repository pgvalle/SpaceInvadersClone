#ifndef SCREENS_INTERNAL_H
#define SCREENS_INTERNAL_H

#include <SDL_types.h>
#include <SDL_rect.h>

#include "screens.h"
#include "../app.h"
#include "../../stb_ds.h"

extern struct menu_screen_t {
    enum {
        MENU_DISPLAYING,
        MENU_WAITING,
        MENU_BLINKING_ON,
        MENU_BLINKING_OFF
    } state;

    int display_i;

    Uint32 timer;
} menu;

void reset_menu();

extern struct pause_screen_t {
    enum {
        PAUSE_BLINKING_ON,
        PAUSE_BLINKING_OFF,
        PAUSE_RESUMING
    } state;

    Uint32 timer;
} pause;

static inline
void reset_pause()
{
    pause.state = PAUSE_BLINKING_ON;
    pause.timer = 0;
}

extern struct over_screen_t {
    enum {
        GAMEOVER_WAITING1,
        GAMEOVER_DISPLAYING,
        GAMEOVER_WAITING2,
        GAMEOVER_BLINKING_ON,
        GAMEOVER_BLINKING_OFF
    } state;

    int display_i;

    Uint32 timer;
} over;

static inline
void reset_over()
{
    over.state = GAMEOVER_WAITING1;
    over.display_i = 0;
    over.timer = 0;
}

///////////////////////////////////////////////////////////////////////////////
// EVERYTHING FROM NOW ON BELONGS TO PLAY SCREEN //
///////////////////////////////////////////////////////////////////////////////

extern struct play_screen_t {
    enum {
        PLAY_PLAYING,
        PLAY_RESTARTING
    } state;

    uint32_t timer;
} play;

void reset_play();

extern struct explosion_t {
    int x, y;
    SDL_Rect clip;
    int32_t lifetime;
}* explosions;

extern struct player_t {
    enum {
        PLAYER_STARTING,
        PLAYER_ALIVE,
        PLAYER_DYING,
        PLAYER_DEAD
    } state;

    int x;
    int lives;
    int dying_clip_swaps;

    SDL_Point* shots;

    uint32_t timer;
} player;

extern struct horde_t {
    enum {
        HORDE_STARTING,
        HORDE_MOVING,
        HORDE_WAITING
    } state;

    int xmove, ymove;

    struct invader_t {
        int x, y;
        SDL_Rect clip;
    }* invaders;

    struct horde_shot_t {
        int x, y;
        SDL_Rect clip;
        uint32_t timer;
    }* shots;

    // One invader is updated each frame.
    // horde is up-to-date only when all invaders are up-to-date.
    int invaders_updated;

    uint32_t timer, shot_timer, shot_timeout;
} horde;

extern struct tourist_t {
    enum {
        TOURIST_AVAILABLE,
        TOURIST_UNAVAILABLE,
        TOURIST_DYING,
        TOURIST_SHOWING_SCORE
    } state;

    float x, xvel;
    int score_inc;
    int available_appearances;

    uint32_t timer, spawn_timeout;
} tourist;

extern struct bunker_t {
    SDL_Rect out_rect;
    SDL_Point points[352];
} bunkers[4];

SDL_Point useless_bar[WIDTH];


#endif // SCREENS_INTERNAL_H
