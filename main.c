#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "stb_ds.h"
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

// resources
SDL_Texture* atlas = NULL, * font_atlas = NULL;

/* APP */

#define FPS 60
#define SCALE 2
#define RESOURCE_DIR "./res"
#define FONT_PTSIZE 8

#define CHARACTERS_MONO " A B C D E F G H  I J K L M N O P Q R S T U V W X"\
	" Y Z 0  1 2 3 4 5 6 7 8 9 - ? <  > = "
#define CHARACTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-?<>="

#define WORLD_WIDTH  224
#define WORLD_HEIGHT 256

struct {
	enum {
		APP_PLAY,
		APP_PAUSE,
        APP_GAMEOVER,
		APP_QUIT
	} screen;

	SDL_Window* window;
    SDL_Event event;

    SDL_Renderer* renderer;

    int credits; // useless. Just an easteregg
    int score, hi_score;

    uint32_t frame_time;
} app;

void render_text_until(const char* text, int x, int y, int n)
{
    const char* characters = CHARACTERS;
    const int len_characters = strlen(CHARACTERS);
    for (int i = 0; i < n; i++)
    {
        // find mapping
        int mapping = 0;
        for (; mapping < len_characters; mapping++)
            if (toupper(text[i]) == characters[mapping]) break;

        if (mapping != len_characters) // success
        {
            SDL_Rect clip = {
                mapping * FONT_PTSIZE,
                0,
                FONT_PTSIZE,
                FONT_PTSIZE
            };
            SDL_Rect scale = {
                SCALE * x,
                SCALE * y,
                SCALE * clip.w,
                SCALE * clip.h
            };

            // Correcting placement of quirky characters
            // I, 1 and = are thinner.
            if (toupper(text[i]) == 'I' || text[i] == '1' || text[i] == '=')
                scale.x -= SCALE;
            // A is just a litle bit off to the right.
            else if (toupper(text[i]) == 'A')
                scale.x += SCALE;

            SDL_RenderCopy(app.renderer, font_atlas, &clip, &scale);
        }

        x += FONT_PTSIZE;
    }
}

static inline
void render_text(const char* text, int x, int y)
{
    render_text_until(text, x, y, strlen(text));
}

void render_clip(const SDL_Rect* clip, int x, int y)
{
    SDL_Rect scale = {
        SCALE * x, SCALE * y, SCALE * clip->w, SCALE * clip->h
    };
    SDL_RenderCopy(app.renderer, atlas, clip, &scale);
}


void process_credit_events()
{
    if (app.event.type != SDL_KEYDOWN)
        return;
    
    switch (app.event.key.keysym.sym)
    {
    case SDLK_PLUS:
    case SDLK_KP_PLUS:
        if (app.credits < 99)
            app.credits++;
        break;
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        if (app.credits > 0)
            app.credits--;
        break;
    }
}

void render_credits()
{
    char credits_str[11];
    sprintf(credits_str, "CREDIT %02d", app.credits);
    render_text(credits_str, WORLD_WIDTH - 80, WORLD_HEIGHT - 16);
}


void render_scores()
{
    static char format[10];
    // score
    render_text("score", 8, 8);
    sprintf(format, "%05d", app.score);
    render_text(format, 8, 24);
    // high-score
    render_text("hi-score", WORLD_WIDTH - 72, 8);
    sprintf(format, "%05d", app.hi_score);
    render_text(format, WORLD_WIDTH - 48, 24);
}


/* STATES STRUCTS AND DEFS (for each app state) */

struct {
    enum {
        GAMEOVER_WAITING,
        GAMEOVER_DISPLAYING,
        GAMEOVER_BLINKING_ON,
        GAMEOVER_BLINKING_OFF
    } state;

    int text_i;

    uint32_t timer;
} over;

struct {
    enum {
        PAUSE_BLINKING_ON,
        PAUSE_BLINKING_OFF,
        PAUSE_RESUMING
    } state;

    uint32_t timer;
} pause;

struct {
    enum {
        PLAY_PLAYING,
        PLAY_RESTARTING
    } state;

    struct explosion_t {
        int x, y;
        SDL_Rect clip;
        uint32_t timer, timeout;
    }* explosions;

    struct {
        enum {
            PLAYER_STARTING,
            PLAYER_ALIVE,
            PLAYER_DYING,
            PLAYER_DEAD
        } state;

        int x;
        int lives;
        int dying_clip_swaps;

        uint32_t timer;
    } player;
    SDL_Point* player_shots;

    struct {
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

        // One invader is updated each frame.
        // horde is up-to-date only when all invaders are up-to-date.
        int invaders_updated;

        uint32_t timer, wait_timer, shot_timeout;
    } horde;
    struct horde_shot_t {
        int x, y;
        SDL_Rect clip;
        uint32_t timer;
    }* horde_shots;

    struct {
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

    uint32_t timer;
} play;


/* GAME OVER STATE */

void reset_over()
{
    over.state = GAMEOVER_WAITING;
    over.text_i = 0;
    over.timer = 0;
}

void reset_play();
void process_over_events()
{
    if (app.event.type != SDL_KEYDOWN || app.event.key.repeat)
        return;
    
    switch (app.event.key.keysym.sym)
    {
    case SDLK_RETURN:
    case SDLK_RETURN2:
        app.screen = APP_PLAY;
        if (app.credits > 0)
            app.credits--;
        reset_play();
        break;
    case SDLK_q:
        app.screen = APP_QUIT;
        break;
    }
}

void update_over()
{
    switch (over.state)
    {
    case GAMEOVER_WAITING:
        over.timer += app.frame_time;
        if (over.timer >= 2000)
        {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_DISPLAYING:
        over.timer += app.frame_time;
        if (over.text_i == 9 && over.timer >= 1008)
            over.state = GAMEOVER_BLINKING_OFF;
        else if (over.text_i < 9 && over.timer >= 160)
        {
            over.text_i++;
            over.timer = 0;
        }
        break;
    case GAMEOVER_BLINKING_ON:
    case GAMEOVER_BLINKING_OFF:
        over.timer += app.frame_time;
        if (over.timer >= 512)
        {
            over.state = (over.state == GAMEOVER_BLINKING_ON ?
                GAMEOVER_BLINKING_OFF : GAMEOVER_BLINKING_ON);
            over.timer = 0;
        }
        break;
    }
}

void render_over()
{
    if (over.state != GAMEOVER_WAITING)
    {
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 200);
        const SDL_Rect overlay_rect = {
            0, 0, SCALE * WORLD_WIDTH, SCALE * WORLD_HEIGHT
        };
        SDL_RenderFillRect(app.renderer, &overlay_rect);
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
    }

    switch (over.state)
    {
    case GAMEOVER_DISPLAYING:
        SDL_SetTextureColorMod(font_atlas, 216, 32, 32);
        render_text_until("YOU LOST", 80, 56, over.text_i);
        SDL_SetTextureColorMod(font_atlas, 255, 255, 255);
        break;
    case GAMEOVER_BLINKING_ON:
        render_text("RETURN - RESTART", 48, 80);
        render_text("Q - QUIT", 80, 96);
    case GAMEOVER_BLINKING_OFF:
        SDL_SetTextureColorMod(font_atlas, 216, 32, 32);
        render_text("YOU LOST", 80, 56);
        SDL_SetTextureColorMod(font_atlas, 255, 255, 255);
        break;
    }
}


/* PAUSE STATE */

void reset_pause()
{
    pause.state = PAUSE_BLINKING_OFF;
    pause.timer = 0;
}

void process_pause_events()
{
    if (app.event.type != SDL_KEYDOWN || app.event.key.repeat)
        return;
    
    switch (app.event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        pause.state = PAUSE_RESUMING;
        break;
    case SDLK_q:
        app.screen = APP_GAMEOVER;
        reset_over();
        break;
    }
}

void update_pause()
{
    switch (pause.state)
    {
    case PAUSE_BLINKING_ON:
    case PAUSE_BLINKING_OFF:
        pause.timer += app.frame_time;
        if (pause.timer >= 512) // 16 * 32 (half second)
        {
            pause.state = (pause.state == PAUSE_BLINKING_ON ?
                PAUSE_BLINKING_OFF : PAUSE_BLINKING_ON);
            pause.timer = 0;
        }
        break;
    case PAUSE_RESUMING:
        pause.timer += app.frame_time;
        if (pause.timer >= 3008) // 16 * 188 (3 seconds)
            app.screen = APP_PLAY;
        break;
    }
}

void render_pause()
{
    // make background darker. It feels like game is really paused
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 200);
    const SDL_Rect overlay_rect = {
        0, 0, SCALE * WORLD_WIDTH, SCALE * WORLD_HEIGHT
    };
    SDL_RenderFillRect(app.renderer, &overlay_rect);
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);

    switch (pause.state)
    {
    case PAUSE_BLINKING_ON:
        render_text("ESC - RESUME", 64, WORLD_HEIGHT / 2 - 16);
        render_text("Q - MENU", 80, WORLD_HEIGHT / 2);
        break;
    case PAUSE_RESUMING: {
        int countdown = 3 - (int)pause.timer / 1000;
        char countdown_str[3];
        sprintf(countdown_str, "%02d", countdown == 0 ? 1 : countdown);
        render_text(countdown_str, WORLD_WIDTH / 2 - 8, WORLD_HEIGHT / 2 - 8);
        break; }
    }
}


/* PLAY STATE */

void update_explosions()
{
    for (int i = 0; i < arrlen(play.explosions); i++)
    {
        play.explosions[i].timer += app.frame_time;
        if (play.explosions[i].timer >= play.explosions[i].timeout)
        {
            arrdel(play.explosions, i);
            i--;
        }
    }
}

void render_explosions()
{
    for (int i = 0; i < arrlen(play.explosions); i++)
        render_clip(
            &play.explosions[i].clip, play.explosions[i].x, play.explosions[i].y
        );
}


void update_player()
{
	switch (play.player.state)
	{
    case PLAYER_STARTING:
        play.player.timer += app.frame_time;
        if (play.player.timer >= 2000)
        {
            play.player.state = PLAYER_ALIVE;
            play.player.x = 14;
            play.player.timer = 0;
        }
        break;
	case PLAYER_ALIVE: {
        const uint8_t* keys = SDL_GetKeyboardState(NULL);
        // moving player //
        if (keys[SDL_SCANCODE_LEFT] && play.player.x > 14)
            play.player.x -= 1;
        if (keys[SDL_SCANCODE_RIGHT] && play.player.x < WORLD_WIDTH - 31)
            play.player.x += 1;

        // shooting mechanic //
        if (play.player.timer < 768)
            play.player.timer += app.frame_time;
        else if (keys[SDL_SCANCODE_SPACE])
        {
            const SDL_Point shot = { play.player.x + 8, 216 };
            arrput(play.player_shots, shot);
            play.player.timer = 0;
        }

        if (keys[SDL_SCANCODE_Q])
        {
            play.player.state = PLAYER_DYING;
            play.player.timer = 0;
        }
        break; }
    case PLAYER_DYING:
        play.player.timer += app.frame_time;
        if (play.player.timer >= 112)
        {
            play.player.dying_clip_swaps++;
            if (play.player.dying_clip_swaps == 9)
            {
                play.player.state = PLAYER_DEAD;
                play.player.lives--;
                play.player.dying_clip_swaps = 0;
            }
            play.player.timer = 0;
        }
        break;
	case PLAYER_DEAD:
		play.player.timer += app.frame_time;
		if (play.player.timer >= 2000)
		{
			play.player.state = PLAYER_ALIVE;
            play.player.x = 14;
			play.player.timer = 0;
		}
		break;
	}
}

void render_player()
{
    switch (play.player.state)
    {
    case PLAYER_DYING: {
        const SDL_Rect clip = {
            (play.player.dying_clip_swaps % 2 + 1) * 16, 8, 16, 8
        };
        render_clip(&clip, play.player.x, 216);
        break; }
    case PLAYER_ALIVE: {
        const SDL_Rect clip = { 0, 8, 16, 8 };
        render_clip(&clip, play.player.x, 216);
        break; }
    }	    
}

void update_player_shots()
{
    for (int i = 0; i < arrlen(play.player_shots); i++)
	{
        // reached top of screen
		play.player_shots[i].y -= 4;
		if (play.player_shots[i].y <= 32)
		{
            // add explosion
            const struct explosion_t explosion = {
                .x = play.player_shots[i].x - 3,
                .y = 34,
                .clip = { 36, 24,  8,  8 },
                .timer = 0,
                .timeout = 16 * 16
            };
            arrput(play.explosions, explosion);
            arrdel(play.player_shots, i);
            i--;
        }
	}
}

void render_player_shots()
{
    for (int i = 0; i < arrlen(play.player_shots); i++)
	{
		SDL_Rect rect = {
			SCALE * play.player_shots[i].x,
			SCALE * play.player_shots[i].y,
			SCALE,
			SCALE * 4,
		};
		SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(app.renderer, &rect);
	}
}


void update_horde_start_anim()
{
    if (play.horde.invaders_updated < 55)
    {
        const int i = 4 - play.horde.invaders_updated / 11; // 4, 3, 2, 1, 0
        const int j = play.horde.invaders_updated % 11; // 0, 1, 2, ..., 10
        // clip rect for invader
        SDL_Rect clip = { 12, 16, 12,  8 };
        if (i > 2) clip.y = 32; // in 4th or 5th row
        else if (i > 0) clip.y = 24; // in 2nd or 3rd row
        const struct invader_t invader = {
            .clip = clip,
            .x = 26 + 16 * j,
            .y = 64 + 16 * i
        };
        arrput(play.horde.invaders, invader);
        play.horde.invaders_updated++;
    }
    else // done. Now move right
    {
        play.horde.state = HORDE_MOVING;
        play.horde.xmove = 2;
        play.horde.ymove = 0;
        play.horde.invaders_updated = 0;
    }
}

void make_horde_shoot()
{
    play.horde.timer += app.frame_time;
    if (play.horde.timer >= play.horde.shot_timeout &&
        arrlen(play.horde.invaders) > 0)
    {
        // someone shoots
        const int i = rand() % arrlen(play.horde.invaders);
        const int x = play.horde.invaders[i].x;
        int y = play.horde.invaders[i].y;
        for (int j = 0; j < i; j++)
        {
            int a = play.horde.invaders[j].x - x;
            a = a < 0 ? -a : a;
            if (a <= 2 && play.horde.invaders[j].y >= play.horde.invaders[i].y)
            {
                y = play.horde.invaders[j].y;
                break;
            }
        }
        struct horde_shot_t shot = {
            .x = x + 5,
            .y = y + 8,
            .clip = { 24, 16 +  8 * (rand() % 3),  3,  7 },
            .timer = 0
        };
        arrput(play.horde_shots, shot);

        // reset
        play.horde.timer = 0;
        play.horde.shot_timeout = 1024 * (rand() % 2 + 1);
    }
}

void move_horde()
{
    if (arrlen(play.horde.invaders) == 0)
        return;
    
    const int i = play.horde.invaders_updated;
    // move this guy
    play.horde.invaders[i].x += play.horde.xmove;
    play.horde.invaders[i].y += play.horde.ymove;
    // animate
    play.horde.invaders[i].clip.x = play.horde.invaders[i].clip.x == 0 ? 12 : 0;
    // now someone got updated
    play.horde.invaders_updated++;
    
    // horde updated and now maybe it's time to flip directions
    int all_updated = play.horde.invaders_updated == arrlen(play.horde.invaders);
    if (all_updated && play.horde.ymove == 0)
    {
        // find out if horde should change direction
        for (int i = 0; i < arrlen(play.horde.invaders); i++)
        {
            const int x = play.horde.invaders[i].x;
            if (x <= 10 || x >= WORLD_WIDTH - 22) // should change directions
            {
                play.horde.ymove = 8;
                play.horde.xmove = -play.horde.xmove;
                break;
            }
        }

        play.horde.invaders_updated = 0; // no one updated now
    }
    else if (all_updated)
    {
        play.horde.ymove = 0;
        play.horde.invaders_updated = 0;
    }
}

void update_horde()
{
    switch (play.horde.state)
    {
    case HORDE_STARTING:
        update_horde_start_anim();
        break;
    case HORDE_MOVING:
        if (play.player.state == PLAYER_ALIVE)
        {
            make_horde_shoot();
            move_horde();
        }
        else if (play.player.state == PLAYER_STARTING)
            move_horde();
        break;
    case HORDE_WAITING:
        play.horde.wait_timer += app.frame_time;
        if (play.horde.wait_timer >= 16 * 24)
        {
            play.horde.state = HORDE_MOVING;
            play.horde.wait_timer = 0;
        }
        break;
    }
}

void update_horde_shots()
{
    for (int i = 0; i < arrlen(play.horde_shots); i++)
	{
        // update shot animation
        play.horde_shots[i].timer += app.frame_time;
        if (play.horde_shots[i].timer >= 16 * 6)
        {
            play.horde_shots[i].clip.x -= 24;
            play.horde_shots[i].clip.x = (play.horde_shots[i].clip.x + 3) % 12;
            play.horde_shots[i].clip.x += 24;
            play.horde_shots[i].timer = 0;
        }

        // reached bottom of screen
		play.horde_shots[i].y++;
		if (play.horde_shots[i].y >= 232)
        {
            // add explosion
            const struct explosion_t explosion = {
                .x = play.horde_shots[i].x - 1,
                .y = 232,
                .clip = { 24, 40,  6,  8 },
                .timer = 0,
                .timeout = 16 * 8
            };
            arrput(play.explosions, explosion);
			arrdel(play.horde_shots, i);
            i--;
        }
	}
}

void render_horde()
{
    for (int i = 0; i < arrlen(play.horde.invaders); i++)
        render_clip(
            &play.horde.invaders[i].clip,
            play.horde.invaders[i].x,
            play.horde.invaders[i].y
        );
}

void render_horde_shots()
{
    for (int i = 0; i < arrlen(play.horde_shots); i++)
        render_clip(
            &play.horde_shots[i].clip,
            play.horde_shots[i].x,
            play.horde_shots[i].y
        );
}


static inline
int gen_tourist_score()
{
    return 100 + 50 * (rand() % 5);
}

static inline
int gen_tourist_spawn_timeout()
{
    return 1024 * (rand() % (30 - 20 + 1) + 20);
}

void update_tourist()
{
    switch (play.tourist.state)
    {
    case TOURIST_AVAILABLE:
        play.tourist.x += play.tourist.xvel;
        // reached end of screen. unavailable
        if (play.tourist.x <= 8.f || play.tourist.x >= WORLD_WIDTH - 32.f)
            play.tourist.state = TOURIST_UNAVAILABLE;
        break;
    case TOURIST_UNAVAILABLE:
        if (play.tourist.available_appearances == 0)
            break;

        play.tourist.timer += app.frame_time;
        if (play.tourist.timer >= play.tourist.spawn_timeout) // spawn
        {
            play.tourist.state = TOURIST_AVAILABLE;
            // spawn either left or right
            play.tourist.xvel = rand() % 2 ? 0.65f : -0.65f;
            play.tourist.x = play.tourist.xvel > 0.0f ? 8.f : (WORLD_WIDTH - 32.f);

            play.tourist.score_inc = gen_tourist_score();
 
            play.tourist.available_appearances--;

            play.tourist.timer = 0; // reset timer
            play.tourist.spawn_timeout = gen_tourist_spawn_timeout();
        }
        break;
    case TOURIST_DYING:
        play.tourist.timer += app.frame_time;
        if (play.tourist.timer >= 384)
        {
            play.tourist.state = TOURIST_SHOWING_SCORE;
            app.score += play.tourist.score_inc;
            play.tourist.timer = 0; // reset timer
        }
        break;
    case TOURIST_SHOWING_SCORE:
        play.tourist.timer += app.frame_time;
        if (play.tourist.timer >= 1280)
        {
            play.tourist.state = TOURIST_UNAVAILABLE;
            play.tourist.timer = 0; // reset timer
        }
        break;
    }
}

void render_tourist()
{
    switch (play.tourist.state)
    {
    case TOURIST_AVAILABLE: {
        const SDL_Rect tourist_clip = { 0,  0, 24,  8 };
        render_clip(&tourist_clip, play.tourist.x, 40);
        break; }
    case TOURIST_DYING: {
        const SDL_Rect tourist_dying = { 24,  0, 24,  8 };
        render_clip(&tourist_dying, play.tourist.x, 40);
        break; }
    case TOURIST_SHOWING_SCORE: {
        char tourist_score[4];
        sprintf(tourist_score, "%d", play.tourist.score_inc);
        SDL_SetTextureColorMod(font_atlas, 216, 32, 32);
        render_text(tourist_score, play.tourist.x, 40);
        SDL_SetTextureColorMod(font_atlas, 255, 255, 255);
        break; }
    }
}


void process_shot_collisions_with_tourist()
{
    for (int i = 0; i < arrlen(play.player_shots); i++)
    {
        const SDL_Rect player_rect = {
            play.player_shots[i].x, play.player_shots[i].y - 4, 1, 1
        };
        const SDL_Rect tourist_rect = {play.tourist.x + 4, 40, 16, 8};
        if (SDL_HasIntersection(&tourist_rect, &player_rect) &&
            play.tourist.state == TOURIST_AVAILABLE)
        {
            play.tourist.state = TOURIST_DYING;
            play.player.lives += rand() % 2;
            
            arrdel(play.player_shots, i);
            i--;
        }
    }
}

void process_shot_collisions_with_player()
{
    for (int i = 0; i < arrlen(play.horde_shots); i++)
    {
        const int hshot_x = play.horde_shots[i].x;
        const int hshot_y = play.horde_shots[i].y;
        // with player
        if (hshot_y >= 216 && hshot_y <= 216 + 6 &&
            hshot_x >= play.player.x + 2 && hshot_x <= play.player.x + 15)
        {
            // create explosion effect
            const struct explosion_t explosion = {
                .x = hshot_x - 1,
                .y = hshot_y,
                .clip = { 24, 40,  6,  8 },
                .timer = 0,
                .timeout = 16 * 24
            };
            arrput(play.explosions, explosion);
            arrdel(play.horde_shots, i);
            i--;

            // player dead now
            play.player.state = PLAYER_DYING;
            play.player.timer = 0;
        }
    }
}

void process_shot_collisions_with_horde()
{
    for (int i = 0; i < arrlen(play.player_shots); i++)
    {
        const SDL_Rect prect = {
            play.player_shots[i].x, play.player_shots[i].y - 4, 1, 1
        };
        for (int j = 0; j < arrlen(play.horde.invaders); j++)
        {
            SDL_Rect irect = {
                play.horde.invaders[j].x + 2, play.horde.invaders[j].y, 8, 8
            };
            int score_inc = 30;
            // 16, 24, 32
            if (j / 11 > 2)
            {
                score_inc = 20;
                irect.x -= 1;
                irect.w += 3;
            }
            else if (j / 11 > 0)
            {
                score_inc = 10;
                irect.x -= 2;
                irect.w += 4;
            }

            if (SDL_HasIntersection(&prect, &irect))
            {
                // add explosion
                const struct explosion_t explosion = {
                    .x = play.horde.invaders[j].x,
                    .y = play.horde.invaders[j].y,
                    .clip = { 0, 40, 13,  8 },
                    .timer = 0,
                    .timeout = 16 * 16
                };
                arrput(play.explosions, explosion);
                arrdel(play.player_shots, i);
                i--;

                if (j <= play.horde.invaders_updated)
                    play.horde.invaders_updated--;
                arrdel(play.horde.invaders, j);
                app.score += score_inc;
                play.horde.state = HORDE_WAITING;
                play.horde.timer = 0;
                break;
            }
        }
    }
}

void process_collisions_between_shots()
{
    for (int i = 0; i < arrlen(play.horde_shots); i++)
    {
        const int hshot_x = play.horde_shots[i].x;
        const int hshot_y = play.horde_shots[i].y;
        for (int j = 0; j < arrlen(play.player_shots); j++) 
        {
            const int pshot_x = play.player_shots[j].x;
            const int pshot_y = play.player_shots[j].y;
            if (pshot_y <= hshot_y && abs(pshot_x - hshot_x) <= 1)
            {
                // horde shot may be stronger than player shot and cut right through it
                if (rand() % 4 != 0)
                {
                    const struct explosion_t hshot_exp = {
                        .x = hshot_x - 1,
                        .y = hshot_y,
                        .clip = { 24, 40,  6,  8 },
                        .timer = 0,
                        .timeout = 16 * 24
                    };
                    arrput(play.explosions, hshot_exp);
                    arrdel(play.horde_shots, i);
                    i--;
                }
                const struct explosion_t pshot_exp = {
                    .x = pshot_x - 3,
                    .y = pshot_y,
                    .clip = { 36, 24,  8,  8 },
                    .timer = 0,
                    .timeout = 16 * 24
                };
                arrput(play.explosions, pshot_exp);
                arrdel(play.player_shots, j);
                break;
            }
        }
    }
}


void reset_play()
{
    app.score = 0;

    play.state = PLAY_PLAYING;

    play.explosions = NULL;

    play.player.state = PLAYER_STARTING;
    play.player.lives = 3;
    play.player.dying_clip_swaps = 0;
    play.player_shots = NULL;
    play.player.timer = 0;

    play.horde.state = HORDE_STARTING;
    play.horde.invaders = NULL;
    play.horde.invaders_updated = 0;
    play.horde_shots = NULL;
    play.horde.timer = 0;
    play.horde.wait_timer = 0;
    play.horde.shot_timeout = 16 * 32 * (rand() % 2 + 1);

    play.tourist.state = TOURIST_UNAVAILABLE;
    play.tourist.available_appearances = rand() % 2 + 2; // 2 or 3 times
    play.tourist.timer = 0;
    play.tourist.spawn_timeout = gen_tourist_spawn_timeout();
}

void process_play_events()
{
    if (app.event.type == SDL_KEYDOWN && !app.event.key.repeat &&
        app.event.key.keysym.sym == SDLK_ESCAPE)
    {
        app.screen = APP_PAUSE;
        reset_pause();
    }
}

void update_play()
{
    switch (play.state)
    {
    case PLAY_PLAYING:
        update_explosions();
        update_player();
        if (play.player.state != PLAYER_DYING &&
            play.player.state != PLAYER_DEAD)
        {
            update_tourist();
            update_horde();
        }

        update_player_shots();
        update_horde_shots();

        process_shot_collisions_with_player();
        process_shot_collisions_with_horde();
        process_shot_collisions_with_tourist();
        process_collisions_between_shots();

        // update score
        if (app.hi_score < app.score)
            app.hi_score = app.score;

        if (play.player.lives == 0 && play.player.state == PLAYER_DEAD)
        {
            app.screen = APP_GAMEOVER;
            reset_over();
        }
        break;
    case PLAY_RESTARTING:
        if (play.timer >= 2000)
        {
            // reset horde partially
            play.horde.state = HORDE_STARTING;
            play.horde.invaders_updated = 0;
            play.horde.timer = 0;
            play.horde.wait_timer = 0;
            play.horde.xmove = 2;
            play.horde.ymove = 0;
            // reseting player partially
            play.player.state = PLAYER_STARTING;
            play.player.x = 14;
            play.player.timer = 0;
            play.state = PLAY_PLAYING;
        }
        break;
    }
}

void render_play()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    // useless stuff
    // bar. Just to resemble the original game
    SDL_SetRenderDrawColor(app.renderer, 32, 255, 32, 255); // #20ff20
    const SDL_Rect rect = {
        SCALE * 0, SCALE * 239, SCALE * 224, SCALE
    };
    SDL_RenderFillRect(app.renderer, &rect);

    render_player_shots();
    render_horde_shots();
    render_tourist();
    render_horde();
    render_player();
    render_explosions();

    // live counter
    char player_lives[] = { '0' + play.player.lives };
    render_text(player_lives, 8, WORLD_HEIGHT - 16);
    // live cannons
    static const SDL_Rect cannon_clip = { 0, 8, 16, 8 };
    for (int i = 0; i < play.player.lives - 1; i++)
        render_clip(&cannon_clip, 24 + i * 16, WORLD_HEIGHT - 16);
}

/* MAIN LOOP AND ENTRY POINT */

void app_main_loop()
{
    uint32_t start = 0, event_wait_timeout = 1000 / FPS;

    while (app.screen != APP_QUIT)
    {
        // beginning of frame. Get current time.
        start = SDL_GetTicks();

        // wait for event
        if (SDL_WaitEventTimeout(&app.event, event_wait_timeout))
        {
            process_credit_events();
            if (app.event.type == SDL_QUIT)
                app.screen = APP_QUIT;
            else switch (app.screen)
            {
            case APP_PLAY:
                process_play_events();
                break;
            case APP_PAUSE:
                process_pause_events();
                break;
            case APP_GAMEOVER:
                process_over_events();
                break;
            }

            // calculate remaining time to wait next loop.
            const uint32_t processing_time = SDL_GetTicks() - start;
            // careful not to be value lower than zero. it's an unsigned int.
            event_wait_timeout = processing_time < event_wait_timeout ?
                (event_wait_timeout - processing_time) : 0;
        }
        else
        {
            switch (app.screen)
            {
            case APP_PAUSE:
                update_pause();
                render_play();
                render_pause();
                break;
            case APP_PLAY:
                update_play();
                render_play();
                break;
            case APP_GAMEOVER:
                update_over();
                render_play();
                render_over();
                break;
            }

            render_scores();
            render_credits();
            SDL_RenderPresent(app.renderer);

            app.frame_time = 0; // reset frame time
            event_wait_timeout = 1000 / FPS; // reset event wait timeout
        }

        // accumulate frame time
        app.frame_time += SDL_GetTicks() - start;
    }
}

int main(int argc, char** args)
{
    // initialization

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    srand(time(NULL));

    SDL_ShowCursor(SDL_DISABLE);

    app.screen = APP_PLAY;
    reset_pause();
    reset_play();
    app.window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCALE * WORLD_WIDTH,
        SCALE * WORLD_HEIGHT,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS
    );
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    app.frame_time = 0;

    app.score = 0;
    app.hi_score = 0;
    // load hi-score
    FILE* hi_score_file = fopen("hi_score.txt", "r");
    if (hi_score_file)
    {
        fscanf(hi_score_file, "%5d", &app.hi_score);
        fclose(hi_score_file);
    }
    else
        fclose(fopen("hi_score.txt", "w"));

    atlas = IMG_LoadTexture(app.renderer, RESOURCE_DIR "/atlas.png");
    SDL_assert(atlas);

    TTF_Init();
    {
        TTF_Font* font = TTF_OpenFont(RESOURCE_DIR "/font.ttf", FONT_PTSIZE);
        SDL_assert(font);

        const SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* font_surf = TTF_RenderUTF8_Solid(font, CHARACTERS_MONO, white);

        // create the texture and free the surface
        font_atlas = SDL_CreateTextureFromSurface(app.renderer, font_surf);
        SDL_FreeSurface(font_surf);

        TTF_CloseFont(font);
    }
    TTF_Quit();
    
    app_main_loop(); // execution

    // termination

    // save score
    hi_score_file = fopen("hi_score.txt", "w");
    fprintf(hi_score_file, "%5d", app.hi_score);
    fclose(hi_score_file);

    SDL_DestroyTexture(font_atlas);
    SDL_DestroyTexture(atlas);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}

/*// bunkers

#define BUNKER_FIRST_X 32
#define BUNKERS_Y 192

SDL_Point bunkers[4][352];

void init_bunker(int k)
{
    // top
    for (int i = 0; i < 4; i++)
    {
        for (int j = 4 - i; j < 18 + i; j++)
        {
            bunkers[k][22 * i + j].x = BUNKER_FIRST_X + 46 * k + j;
            bunkers[k][22 * i + j].y = BUNKERS_Y + i;
        }
    }

    // middle
    for (int i = 4; i < 12; i++)
    {
        for (int j = 0; j < 22; j++)
        {
            bunkers[k][22 * i + j].x = BUNKER_FIRST_X + 46 * k + j;
            bunkers[k][22 * i + j].y = BUNKERS_Y + i;
        }
    }

    for (int j = 0; j < 8; j++)
    {
        bunkers[k][22 * 12 + j].x = BUNKER_FIRST_X + 46 * k + j;
        bunkers[k][22 * 12 + j].y = BUNKERS_Y + 12;

        bunkers[k][22 * 12 + j + 14].x = BUNKER_FIRST_X + 46 * k + j + 14;
        bunkers[k][22 * 12 + j + 14].y = BUNKERS_Y + 12;
    }

    for (int j = 0; j < 7; j++)
    {
        bunkers[k][22 * 13 + j].x = BUNKER_FIRST_X + 46 * k + j;
        bunkers[k][22 * 13 + j].y = BUNKERS_Y + 13;

        bunkers[k][22 * 13 + j + 15].x = BUNKER_FIRST_X + 46 * k + j + 15;
        bunkers[k][22 * 13 + j + 15].y = BUNKERS_Y + 13;
    }

    for (int i = 14; i < 16; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            bunkers[k][22 * i + j].x = BUNKER_FIRST_X + 46 * k + j;
            bunkers[k][22 * i + j].y = BUNKERS_Y + i;

            bunkers[k][22 * i + j + 16].x = BUNKER_FIRST_X + 46 * k + j + 16;
            bunkers[k][22 * i + j + 16].y = BUNKERS_Y + i;
        }
    }
}

void init_bunkers()
{
    init_bunker(0);
    init_bunker(1);
    init_bunker(2);
    init_bunker(3);
}

void render_bunkers()
{
    SDL_SetRenderDrawColor(app.renderer, 32, 255, 32, 255);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 352; j++)
        {
            const SDL_Rect piece_rect = {
                bunkers[i][j].x * SCALE,
                bunkers[i][j].y * SCALE,
                SCALE,
                SCALE
            };
            SDL_RenderFillRect(app.renderer, &piece_rect);
        }
    }
}*/

/*

// horde

#define HORDE_SIZE 55

struct {
    enum {
        HORDE_STARTING, // start animation
        HORDE_WAITING
    } state;

    int xvel;
    int xclip;

    struct invader_t {
		enum {
			INVADER_ALIVE,
			INVADER_DEAD
		} state;

		int x, y;
    } invaders[HORDE_SIZE];

    struct horde_shot_t {
        int x, y;
        SDL_Rect clip;
        uint32_t timer;
    }* shots;

    uint32_t timer;

    // starting state variables //

    uint8_t start_anim_frames;

    // waiting state variables //

    uint32_t waiting_timeout, shot_timeout;
} horde;

void init_horde()
{
    horde.state = HORDE_STARTING;
    
    horde.xvel = 2;
    horde.xclip = 12;

    for (int i = 0; i < HORDE_SIZE; i++)
    {
        const int y = 4 - i / 11;
        const int x = i % 11;
        horde.invaders[i] = (struct invader_t){
            .state = INVADER_DEAD,
			.x = 26 + 16 * x,
            .y = 64 + 16 * y
        };
    }
    horde_shots = NULL;

    horde.timer = 0;
    horde.start_anim_frames = 0;
    horde.waiting_timeout = 0;
    horde.shot_timeout = 0;
}

void move_horde()
{
    int yvel = 0;
    for (int i = 0; i < arrlen(horde.invaders); i++)
    {
        const int x = horde.invaders[i].x;
        if (x <= 10 || x >= WORLD_WIDTH - 22)
        {
            horde.xvel = -horde.xvel;
            yvel = 8;
            break;
        }
    }
    
    for (int i = 0; i < arrlen(horde.invaders); i++)
    {
        horde.invaders[i].x += horde.xvel;
        horde.invaders[i].y += yvel;
    }
}

void make_horde_shoot()
{
    horde.timer += app.frame_time;
    if (horde.timer >= horde.shot_timeout && arrlen(horde.invaders) > 0)
    {
        // someone shoots
        const int i = rand() % arrlen(horde.invaders);
        const int x = horde.invaders[i].x;
        int y = horde.invaders[i].y;
        for (int j = 0; j < i; j++)
        {
            int a = horde.invaders[j].x - x;
            a = a < 0 ? -a : a;
            if (a <= 2 && horde.invaders[j].y >= horde.invaders[i].y)
            {
                y = horde.invaders[j].y;
                break;
            }
        }
        struct horde_shot_t shot = {
            .x = x + 5,
            .y = y + 8,
            .clip = { 24, 16 +  8 * (rand() % 3),  3,  7 },
            .timer = 0
        };
        arrput(horde_shots, shot);

        // reset
        horde.timer = 0;
        horde.shot_timeout = 1024 * (rand() % 2 + 1);
    }
}

void update_horde()
{
    switch (horde.state)
    {
    case HORDE_STARTING:
        horde.timer += app.frame_time;
        if (horde.timer >= 16)
        {
            horde.start_anim_frames++;
            if (horde.start_anim_frames == 55)
                horde.state = HORDE_WAITING;
            horde.timer = 0;
        }
        break;
    case HORDE_WAITING:
        horde.timer += app.frame_time;
        if (horde.timer >= horde.waiting_timeout)
        {
            move_horde();
            // animate
        	horde.xclip = horde.xclip == 12 ? 0 : 12;
			horde.timer = 0;
        }
        break;
    }
}

void render_horde()
{
    for (int i = 0; i < arrlen(horde.invaders); i++)
	{
		SDL_Rect clip = { horde.xclip, 16, 12, 8 };
		if (i / 11 > 2) clip.y = 32; // in 4th or 5th row
        else if (i / 11 > 0) clip.y = 24; // in 2nd or 3rd row
        render_clip(&clip, horde.invaders[i].x, horde.invaders[i].y);
	}
}

*/


// for (int i = 0; i < arrlen(horde_shots); i++)
//     render_clip(&horde_shots[i].clip, horde_shots[i].x, horde_shots[i].y);

/*void update_shots()
{
    // horde shots
    

    // player shots
    
}
*/
