#ifndef PLAY_H
#define PLAY_H

static struct {
  enum {
    PLAY_PLAYING,
    PLAY_RESTARTING
  } state;

  Uint32 timer;
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

    Uint32 timer;
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
        Uint32 timer;
    }* shots;

    // One invader is updated each frame.
    // horde is up-to-date only when all invaders are up-to-date.
    int invaders_updated;

    Uint32 timer, shot_timer, shot_timeout;
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

    Uint32 timer, spawn_timeout;
} tourist;

extern struct bunker_t {
    SDL_Rect out_rect;
    SDL_Point points[352];
} bunkers[4];

#endif // PLAY_H