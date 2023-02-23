#include "app.h"
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#undef STB_DS_IMPLEMENTATION


void process_credit_events(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN)
        return;
    
    switch (event->key.keysym.sym)
    {
    case SDLK_PLUS:
    case SDLK_KP_PLUS:
        if (credits < 99)
            credits++;
        break;
    case SDLK_MINUS:
    case SDLK_KP_MINUS:
        if (credits > 0)
            credits--;
        break;
    }
}

void render_credits()
{
    char credit_text[10];
    sprintf(credit_text, "CREDIT %02d", credits);
    render_text(credit_text, 9, WIDTH - 80, HEIGHT - 16);
}


void render_scores()
{
    char score_text[7];
    // score
    render_text("YOUR SCORE", 10, 8, 8);
    sprintf(score_text, "%06d", score);
    render_text(score_text, 6, 24, 24);
    // high-score
    render_text("HIGH-SCORE", 10, WIDTH - 88, 8);
    sprintf(score_text, "%06d", hi_score);
    render_text(score_text, 6, WIDTH - 72, 24);
}

void render_score_advances_table()
{
    render_text("*SCORE ADVANCES TABLE*", 22, 24, 128);

    const SDL_Rect tourist_clip = { 0,  0, 24,  8 };
    render_clip(&tourist_clip, 60, 152);
    render_text("=? MYSTERY", 10, 80, 152);

    const SDL_Rect invader1_clip = { 0, 16, 12,  8 };
    render_clip(&invader1_clip, 66, 168);
    render_text("=30 POINTS", 10, 80, 168);

    const SDL_Rect invader2_clip = { 0, 24, 12,  8 };
    render_clip(&invader2_clip, 66, 184);
    render_text("=20 POINTS", 10, 80, 184);

    const SDL_Rect invader3_clip = { 0, 32, 12,  8 };
    render_clip(&invader3_clip, 66, 200);
    render_text("=10 POINTS", 10, 80, 200);
}


// SCREENS STRUCTS (for each app screen) //

struct {
    enum {
        MENU_DISPLAYING,
        MENU_WAITING,
        MENU_BLINKING_ON,
        MENU_BLINKING_OFF
    } state;

    int display_i;

    uint32_t timer;
} menu;

struct {
    enum {
        GAMEOVER_WAITING1,
        GAMEOVER_DISPLAYING,
        GAMEOVER_WAITING2,
        GAMEOVER_BLINKING_ON,
        GAMEOVER_BLINKING_OFF
    } state;

    int display_i;

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

/* ALL PLAY SCREEN STRUCTS */

struct {
    enum {
        PLAY_PLAYING,
        PLAY_RESTARTING
    } state;

    uint32_t timer;
} play;

struct explosion_t {
    int x, y;
    SDL_Rect clip;
    int32_t lifetime;
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

    SDL_Point* shots;

    uint32_t timer;
} player;

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

struct {
    SDL_Rect out_rect;
    SDL_Point points[352];
} bunkers[4];

SDL_Point useless_bar[WIDTH];


// MENU SCREEN IMPLEMENTATION //

void reset_menu()
{
    menu.state = MENU_DISPLAYING;
    menu.display_i = 0;
    menu.timer = 0;
}

void reset_play();
void process_menu_events(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN || event->key.repeat)
        return;
    
    SDL_Keycode key = event->key.keysym.sym;
    switch (menu.state)
    {
    case MENU_DISPLAYING:
    case MENU_WAITING:
        if (key == SDLK_RETURN)
        {
            menu.state = MENU_BLINKING_ON;
            menu.timer = 0;
        }
        break;
    case MENU_BLINKING_ON:
    case MENU_BLINKING_OFF:
        if (key == SDLK_p)
        {
            screen = SCREEN_PLAY;
            if (credits > 0)
                credits--;
            reset_play();
        }
        else if (key == SDLK_q)
            screen = SCREEN_EXIT;
        break;
    }
}

void update_menu(Uint32 delta)
{
    switch (menu.state)
    {
    case MENU_DISPLAYING:
        menu.timer += delta;
        if (menu.timer >= 160)
        {
            if (++menu.display_i == 14)
                menu.state = MENU_WAITING;
            menu.timer = 0;
        }
        break;
    case MENU_WAITING:
        menu.timer += delta;
        if (menu.timer >= 1008)
        {
            menu.state = MENU_BLINKING_ON;
            menu.timer = 0;
        }
        break;
    case MENU_BLINKING_ON:
    case MENU_BLINKING_OFF:
        menu.timer += delta;
        if (menu.timer >= 498)
        {
            menu.state = (menu.state == MENU_BLINKING_ON ?
                MENU_BLINKING_OFF : MENU_BLINKING_ON);
            menu.timer = 0;
        }
        break;
    }
}

void render_menu()
{
    switch (menu.state)
    {
    case MENU_BLINKING_ON:
        render_text("<P> PLAY", 8, 80, 80);
        render_text("<Q> QUIT", 8, 80, 96);
    case MENU_BLINKING_OFF:
        render_text("SPACE INVADERS", 14, 56, 56);
        render_score_advances_table();
        break;
    case MENU_WAITING:
    case MENU_DISPLAYING:
        render_text("SPACE INVADERS", menu.display_i, 56, 56);
        break;
    }
}


// GAME OVER SCREEN IMPLEMENTATION //

void reset_over()
{
    over.state = GAMEOVER_WAITING1;
    over.display_i = 0;
    over.timer = 0;
}

void reset_play();
void process_over_events(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN || event->key.repeat)
        return;

    SDL_Keycode key = event->key.keysym.sym;
    switch (over.state)
    {
    case GAMEOVER_WAITING1:
        if (key == SDLK_RETURN)
        {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_WAITING2:
    case GAMEOVER_DISPLAYING:
        if (key == SDLK_RETURN)
        {
            over.state = GAMEOVER_BLINKING_ON;
            over.timer = 0;
        }
        break;
    case GAMEOVER_BLINKING_ON:
    case GAMEOVER_BLINKING_OFF:
        switch (key)
        {
        case SDLK_RETURN:
        case SDLK_RETURN2:
            screen = SCREEN_PLAY;
            if (credits > 0)
                credits--;
            reset_play();
            break;
        case SDLK_q:
            screen = SCREEN_EXIT;
            break;
        case SDLK_m:
            screen = SCREEN_MENU;
            reset_menu();
            break;
        }
        break;
    }
}

void update_over(Uint32 delta)
{
    switch (over.state)
    {
    case GAMEOVER_WAITING1:
        over.timer += delta;
        if (over.timer >= 2000)
        {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_DISPLAYING:
        over.timer += delta;
        if (over.timer >= 160)
        {
            if (++over.display_i == 8)
                over.state = GAMEOVER_WAITING2;
            over.timer = 0;
        }
        break;
    case GAMEOVER_WAITING2:
        over.timer += delta;
        if (over.timer >= 1008)
        {
            over.state = GAMEOVER_BLINKING_ON;
            over.timer = 0;
        }
        break;
    case GAMEOVER_BLINKING_ON:
    case GAMEOVER_BLINKING_OFF:
        over.timer += delta;
        if (over.timer >= 498)
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
    if (over.state != GAMEOVER_WAITING1)
    {
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 225);
        const SDL_Rect overlay_rect = { 0, 0, WIDTH, HEIGHT };
        SDL_RenderFillRect(ren, &overlay_rect);
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    }

    switch (over.state)
    {
    case GAMEOVER_WAITING2:
    case GAMEOVER_DISPLAYING:
        SDL_SetTextureColorMod(atlas, 216, 32, 32);
        render_text("YOU LOST", over.display_i, 80, 56);
        SDL_SetTextureColorMod(atlas, 255, 255, 255);
        break;
    case GAMEOVER_BLINKING_ON:
        render_text("<ENTER> GO AGAIN", 16, 48, 80);
        render_text("<M> MENU", 8, 80, 96);
        render_text("<Q> QUIT", 8, 80, 112);
    case GAMEOVER_BLINKING_OFF:
        SDL_SetTextureColorMod(atlas, 216, 32, 32);
        render_text("YOU LOST", 8, 80, 56);
        SDL_SetTextureColorMod(atlas, 255, 255, 255);
        break;
    }
}


// PAUSE SCREEN IMPLEMENTATION //

void reset_pause()
{
    pause.state = PAUSE_BLINKING_ON;
    pause.timer = 0;
}

void process_pause_events(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN || event->key.repeat ||
        pause.state == PAUSE_RESUMING)
        return;
    
    switch (event->key.keysym.sym)
    {
    case SDLK_m:
        screen = SCREEN_MENU;
        score = 0;
        reset_menu();
        break;
    case SDLK_ESCAPE:
        pause.state = PAUSE_RESUMING;
        pause.timer = 0;
        break;
    case SDLK_q:
        screen = SCREEN_EXIT;
        reset_over();
        break;
    }
}

void update_pause(Uint32 delta)
{
    switch (pause.state)
    {
    case PAUSE_BLINKING_ON:
    case PAUSE_BLINKING_OFF:
        pause.timer += delta;
        if (pause.timer >= 498) // 16 * 32 (half second)
        {
            pause.state = (pause.state == PAUSE_BLINKING_ON ?
                PAUSE_BLINKING_OFF : PAUSE_BLINKING_ON);
            pause.timer = 0;
        }
        break;
    case PAUSE_RESUMING:
        pause.timer += delta;
        if (pause.timer >= 3008) // 16 * 188 (3 seconds)
            screen = SCREEN_PLAY;
        break;
    }
}

void render_pause()
{
    // make background darker. It feels like game is really paused
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 225);
    const SDL_Rect overlay_rect = { 0, 0, WIDTH, HEIGHT };
    SDL_RenderFillRect(ren, &overlay_rect);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);

    switch (pause.state)
    {
    case PAUSE_BLINKING_ON:
        render_text("<ESC> RESUME", 12, 64, 64);
        render_text("<M> MENU", 8, 80, 80);
        render_text("<Q> QUIT", 8, 80, 96);
    case PAUSE_BLINKING_OFF:
        render_score_advances_table();
        break;
    case PAUSE_RESUMING: {
        int countdown = 3 - (int)pause.timer / 1000;
        char countdown_text[3];
        sprintf(countdown_text, "%02d", countdown == 0 ? 1 : countdown);
        render_text(countdown_text, 2, WIDTH / 2 - 8, 112);
        break; }
    }
}


// PLAY SCREEN IMPLEMENTATION //

// explosions //

void update_explosions(Uint32 delta)
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        explosions[i].lifetime -= delta;
        if (explosions[i].lifetime <= 0)
        {
            arrdel(explosions, i);
            i--;
        }
    }
}

void render_explosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
        render_clip(&explosions[i].clip, explosions[i].x, explosions[i].y);
}


// player //

void update_player(Uint32 delta)
{
	switch (player.state)
	{
    case PLAYER_STARTING:
        player.timer += delta;
        if (player.timer >= 2000)
        {
            player.state = PLAYER_ALIVE;
            player.x = 14;
            player.timer = 0;
        }
        break;
	case PLAYER_ALIVE: {
        const uint8_t* keys = SDL_GetKeyboardState(NULL);
        // moving player
        if (keys[SDL_SCANCODE_LEFT] && player.x > 14)
            player.x -= 1;
        if (keys[SDL_SCANCODE_RIGHT] && player.x < WIDTH - 31)
            player.x += 1;

        // shooting mechanic
        if (player.timer < 768)
            player.timer += delta;
        else if (keys[SDL_SCANCODE_SPACE])
        {
            const SDL_Point shot = { player.x + 8, 216 };
            arrput(player.shots, shot);
            player.timer = 0;
        }

        // check if horde has reached player
        if (arrlen(horde.invaders) > 0 && horde.invaders[0].y == 216)
        {
            player.state = PLAYER_DYING;
            player.timer = 0;
        }
        break; }
    case PLAYER_DYING:
        player.timer += delta;
        if (player.timer >= 112)
        {
            if (++player.dying_clip_swaps == 9)
            {
                player.state = PLAYER_DEAD;
                player.lives--;
                player.dying_clip_swaps = 0;
            }
            player.timer = 0;
        }
        break;
	case PLAYER_DEAD:
		player.timer += delta;
		if (player.timer >= 2000)
		{
            // horde reached player. game over
            if (horde.invaders[0].y == 216)
            {
                screen = SCREEN_GAMEOVER;
                reset_over();
            }
            else
            {
                player.state = PLAYER_ALIVE;
                player.x = 14;
                player.timer = 0;
            }
		}
		break;
	}
}

void render_player()
{
    switch (player.state)
    {
    case PLAYER_DYING: {
        const SDL_Rect clip = {
            (player.dying_clip_swaps % 2 + 1) * 16, 8, 16, 8
        };
        render_clip(&clip, player.x, 216);
        break; }
    case PLAYER_ALIVE: {
        const SDL_Rect clip = { 0, 8, 16, 8 };
        render_clip(&clip, player.x, 216);
        break; }
    }	    
}

void update_player_shots()
{
    for (int i = 0; i < arrlen(player.shots); i++)
	{
        // reached top of screen
		player.shots[i].y -= 4;
		if (player.shots[i].y <= 32)
		{
            // add explosion
            const struct explosion_t explosion = {
                .x = player.shots[i].x - 3,
                .y = 34,
                .clip = { 36, 24,  8,  8 },
                .lifetime = 256
            };
            arrput(explosions, explosion);
            arrdel(player.shots, i);
            i--;
        }
	}
}

void render_player_shots()
{
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for (int i = 0; i < arrlen(player.shots); i++)
	{
		const SDL_Rect shot_rect = {
			player.shots[i].x, player.shots[i].y, 1, 4
		};
		SDL_RenderFillRect(ren, &shot_rect);
	}
}


// horde //

static inline
uint32_t gen_horde_shot_timeout()
{
    return 498 * (rand() % 2 + 1);
}

void update_horde_start_anim()
{
    if (horde.invaders_updated < 55)
    {
        const int i = 4 - horde.invaders_updated / 11; // 4, 3, 2, 1, 0
        const int j = horde.invaders_updated % 11; // 0, 1, 2, ..., 10
        // clip rect for invader
        SDL_Rect clip = { 12, 16, 12,  8 };
        if (i > 2) clip.y = 32; // in 4th or 5th row
        else if (i > 0) clip.y = 24; // in 2nd or 3rd row
        const struct invader_t invader = {
            .clip = clip,
            .x = 26 + 16 * j,
            .y = 64 + 16 * i
        };
        arrput(horde.invaders, invader);
        horde.invaders_updated++;
    }
    else // done. Now move right
    {
        horde.state = HORDE_MOVING;
        horde.xmove = 2;
        horde.ymove = 0;
        horde.invaders_updated = 0;
        horde.timer = 0;
        horde.shot_timeout = gen_horde_shot_timeout();
    }
}

void make_horde_shoot()
{
    if (horde.shot_timer < horde.shot_timeout || arrlen(horde.invaders) == 0)
        return;

    // reset shot timing variables
    horde.shot_timer = 0;
    horde.shot_timeout = gen_horde_shot_timeout();

    // 1/2 change of being a random shot
    if (rand() % 2 == 0)
    {
        // pick a random invader to shoot
        const int i = rand() % arrlen(horde.invaders);
        for (int j = 0; j <= i; j++)
        {
            // horizontal distance < 3 => invader j in same column as invader i
            if (abs(horde.invaders[i].x- horde.invaders[j].x) < 3)
            {
                struct horde_shot_t shot = {
                    .x = horde.invaders[i].x + 5,
                    .y = horde.invaders[j].y + 8,
                    .clip = { 24, 8 * (rand() % 3 + 2),  3,  7 },
                    .timer = 0
                };
                arrput(horde.shots, shot);
                return;
            }
        }
    }

    // danger zone width is 5. Necessarily 1 column of invaders will be in it
    SDL_Rect danger_zone = { player.x + 6, 0, 5, HEIGHT };
    for (int i = 0; i < arrlen(horde.invaders); i++)
    {
        const SDL_Rect invader_rect = {
            horde.invaders[i].x, horde.invaders[i].y, 12, 8
        };
        if (SDL_HasIntersection(&danger_zone, &invader_rect))
        {
            struct horde_shot_t shot = {
                .x = invader_rect.x + 5,
                .y = invader_rect.y + 8,
                .clip = { 24, 16 +  8 * (rand() % 3),  3,  7 },
                .timer = 0
            };
            arrput(horde.shots, shot);
            break;
        }
    }
}

void move_horde()
{
    if (arrlen(horde.invaders) == 0)
        return;

    horde.invaders_updated %= arrlen(horde.invaders);
    const int i = horde.invaders_updated; // index of invader to update
    // move
    horde.invaders[i].x += horde.xmove;
    horde.invaders[i].y += horde.ymove;
    horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0; // animate
    horde.invaders_updated++; // +1 updated

    // horde partially updated so we're done.
    if (horde.invaders_updated < arrlen(horde.invaders))
        return;
    // fully updated and moved down. Don't move down next time
    if (horde.ymove != 0)
    {
        horde.ymove = 0;
        return;
    }

    // fully updated and didn't move down. Maybe it's time to flip directions
    for (int j = 0; j < arrlen(horde.invaders); j++)
    {
        // should change directions and move down
        const int x = horde.invaders[j].x;
        if (x <= 10 || x >= WIDTH - 22)
        {
            horde.xmove = -horde.xmove;
            horde.ymove = 8;
            break;
        }
    }
}

void update_horde(Uint32 delta)
{
    switch (horde.state)
    {
    case HORDE_STARTING:
        update_horde_start_anim();
        break;
    case HORDE_MOVING:
        if (player.state == PLAYER_ALIVE)
        {
            horde.shot_timer += delta;
            make_horde_shoot();
        }
        if (player.state == PLAYER_STARTING || player.state == PLAYER_ALIVE)
            move_horde();
        break;
    case HORDE_WAITING:
        horde.timer += delta;
        if (horde.timer >= 256)
            horde.state = HORDE_MOVING;
        break;
    }
}

void update_horde_shots(Uint32 delta)
{
    for (int i = 0; i < arrlen(horde.shots); i++)
	{
        // update shot animation
        horde.shots[i].timer += delta;
        if (horde.shots[i].timer >= 96)
        {
            horde.shots[i].clip.x = 24 + (horde.shots[i].clip.x - 21) % 12;
            horde.shots[i].timer = 0;
        }

        // reached bottom of screen
		horde.shots[i].y += 2;
		if (horde.shots[i].y >= 232)
        {
            // take away some bar pixels
            useless_bar[horde.shots[i].x] = (SDL_Point){ -1, -1 };
            useless_bar[horde.shots[i].x + 2] = (SDL_Point){ -1, -1 };
            useless_bar[horde.shots[i].x + 4] = (SDL_Point){ -1, -1 };

            // add explosion
            const struct explosion_t explosion = {
                .x = horde.shots[i].x - 1,
                .y = 232,
                .clip = { 37, 32,  6,  8 },
                .lifetime = 128
            };
            arrput(explosions, explosion);
			arrdel(horde.shots, i);
            i--;
        }
	}
}

void render_horde()
{
    for (int i = 0; i < arrlen(horde.invaders); i++)
    {
        const struct invader_t invader = horde.invaders[i];
        render_clip(&invader.clip, invader.x, invader.y);
    }
}

void render_horde_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
    {
        const struct horde_shot_t shot = horde.shots[i];
        render_clip(&shot.clip, shot.x, shot.y);
    }
}


// tourist //

static inline
int gen_tourist_score()
{
    return 100 * (rand() % 3 + 1);
}

static inline
int gen_tourist_spawn_timeout()
{
    return 1008 * (rand() % (30 - 20 + 1) + 20);
}

void update_tourist(Uint32 delta)
{
    switch (tourist.state)
    {
    case TOURIST_AVAILABLE:
        tourist.x += tourist.xvel;
        // reached end of screen. unavailable
        if (tourist.x <= 8.f || tourist.x >= WIDTH - 32.f)
            tourist.state = TOURIST_UNAVAILABLE;
        break;
    case TOURIST_UNAVAILABLE:
        if (tourist.available_appearances == 0)
            break;

        tourist.timer += delta;
        if (tourist.timer >= tourist.spawn_timeout) // spawn
        {
            tourist.state = TOURIST_AVAILABLE;
            // spawn either left or right
            tourist.xvel = rand() % 2 ? 0.65f : -0.65f;
            tourist.x = tourist.xvel > 0.0f ? 8.f : (WIDTH - 32.f);

            tourist.score_inc = gen_tourist_score();
 
            tourist.available_appearances--;

            tourist.timer = 0; // reset timer
            tourist.spawn_timeout = gen_tourist_spawn_timeout();
        }
        break;
    case TOURIST_DYING:
        tourist.timer += delta;
        if (tourist.timer >= 384)
        {
            tourist.state = TOURIST_SHOWING_SCORE;
            score += tourist.score_inc;
            tourist.timer = 0; // reset timer
        }
        break;
    case TOURIST_SHOWING_SCORE:
        tourist.timer += delta;
        if (tourist.timer >= 1280)
        {
            tourist.state = TOURIST_UNAVAILABLE;
            tourist.timer = 0; // reset timer
        }
        break;
    }
}

void render_tourist()
{
    switch (tourist.state)
    {
    case TOURIST_AVAILABLE: {
        const SDL_Rect tourist_clip = { 0,  0, 24,  8 };
        render_clip(&tourist_clip, tourist.x, 40);
        break; }
    case TOURIST_DYING: {
        const SDL_Rect tourist_dying = { 24,  0, 24,  8 };
        render_clip(&tourist_dying, tourist.x, 40);
        break; }
    case TOURIST_SHOWING_SCORE: {
        char tourist_score[4];
        sprintf(tourist_score, "%3d", tourist.score_inc);
        SDL_SetTextureColorMod(atlas, 216, 32, 32);
        render_text(tourist_score, 3, tourist.x, 40);
        SDL_SetTextureColorMod(atlas, 255, 255, 255);
        break; }
    }
}


// bunkers //

void desintegrate_bunker_from_point(int b, int p)
{
    // offsets from center to remove
    const SDL_Point offsets[26] = {
        {  0, -4 },
        { -2, -3 }, {  2, -3 },
        {  0, -2 }, {  1, -2 }, {  3, -2 },
        { -1, -1 },
        {  0, -1 }, {  1, -1 }, {  2, -1 },
        { -2,  0 }, {  0,  0 }, {  1,  0 }, {  2,  0 },
        { -1,  1 }, {  0,  1 }, {  1,  1 }, {  2,  1 }, {  3,  1 },
        { -2,  2 },
        {  0,  2 }, {  1,  2 }, {  2,  2 },
        { -1,  3 }, {  1,  3 }, {  3,  3 },
    };

    for (int i = 0; i < 26; i++)
    {
        const int x = p % 22 + offsets[i].x;
        const int y = p / 22 + offsets[i].y;
        if (0 <= x && x < 22 && 0 <= y && y < 16)
        {
            bunkers[b].points[22 * y + x].x = -1;
            bunkers[b].points[22 * y + x].y = -1;
        }
    }
}

void render_bunkers()
{
    SDL_SetRenderDrawColor(ren, 32, 255, 32, 255);

    SDL_RenderDrawPoints(ren, bunkers[0].points, 352);
    SDL_RenderDrawPoints(ren, bunkers[1].points, 352);
    SDL_RenderDrawPoints(ren, bunkers[2].points, 352);
    SDL_RenderDrawPoints(ren, bunkers[3].points, 352);
}


// collisions //

void process_shot_collision_with_tourist()
{
    const SDL_Rect tourist_rect = { tourist.x + 4, 40, 16, 8 };
    for (int i = 0; i < arrlen(player.shots); i++)
    {
        const SDL_Rect shot_rect = {
            player.shots[i].x, player.shots[i].y - 4, 1, 1
        };
        if (SDL_HasIntersection(&tourist_rect, &shot_rect) &&
            tourist.state == TOURIST_AVAILABLE)
        {            
            arrdel(player.shots, i);
            i--;

            tourist.state = TOURIST_DYING;
            // 50% chance of getting +1 life if tourist gives +300 points
            player.lives += tourist.score_inc == 300 ? rand() % 2 : 0;
            break;
        }
    }
}

void process_shot_collision_with_player()
{
    const SDL_Rect player_rect = { player.x + 2, 216, 13, 8 };
    for (int i = 0; i < arrlen(horde.shots); i++)
    {
        const SDL_Rect shot_rect = {
            horde.shots[i].x + 1, horde.shots[i].y, 1, 1
        };
        if (SDL_HasIntersection(&player_rect, &shot_rect) &&
            player.state != PLAYER_DEAD)
        {
            arrdel(horde.shots, i);
            i--;
            // player dead now
            player.state = PLAYER_DYING;
            player.timer = 0;
            break;
        }
    }
}

void process_shot_collision_with_horde()
{
    for (int i = 0; i < arrlen(player.shots); i++)
    {
        const SDL_Rect shot_rect = {
            player.shots[i].x, player.shots[i].y - 4, 1, 1
        };
        for (int j = 0; j < arrlen(horde.invaders); j++)
        {
            SDL_Rect invader_rect = {
                horde.invaders[j].x + 2, horde.invaders[j].y, 8, 8
            };
            int score_inc = 30;
            // 16, 24, 32
            if (horde.invaders[j].clip.y == 24)
            {
                score_inc = 20;
                invader_rect.x -= 1;
                invader_rect.w = 11;
            }
            else if (horde.invaders[j].clip.y == 32)
            {
                score_inc = 10;
                invader_rect.x -= 2;
                invader_rect.w = 12;
            }

            if (SDL_HasIntersection(&shot_rect, &invader_rect))
            {
                const struct explosion_t explosion = {
                    .x = horde.invaders[j].x,
                    .y = horde.invaders[j].y,
                    .clip = { 24, 32, 13,  8 },
                    .lifetime = 256
                };
                arrput(explosions, explosion);
                arrdel(player.shots, i);
                i--;
                arrdel(horde.invaders, j);
                score += score_inc;

                // prevent updating twice the same invader (corner case)
                if (j < horde.invaders_updated)
                    horde.invaders_updated--;
                
                // horde now needs to wait (make delay effect)
                horde.state = HORDE_WAITING;
                horde.timer = 0;
                return;
            }
        }
    }
}

void process_collision_between_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
    {
        const SDL_Rect horde_shot_rect = {
            horde.shots[i].x, horde.shots[i].y, 3, 8
        };
        for (int j = 0; j < arrlen(player.shots); j++) 
        {
            const SDL_Rect player_shot_rect = {
                player.shots[j].x, player.shots[j].y, 1, 5
            };
            if (SDL_HasIntersection(&horde_shot_rect, &player_shot_rect))
            {
                // 25% chance horde shot cut right through player shot
                if (rand() % 4 != 0) // 75% it explodes as well
                {
                    const struct explosion_t horde_shot_explosion = {
                        .x = horde_shot_rect.x - 2,
                        .y = horde_shot_rect.y + 1,
                        .clip = { 37, 32,  6,  8 },
                        .lifetime = 384
                    };
                    arrput(explosions, horde_shot_explosion);
                    arrdel(horde.shots, i);
                    i--;
                }

                const struct explosion_t player_shot_explosion = {
                    .x = player_shot_rect.x - 3,
                    .y = player_shot_rect.y - 1,
                    .clip = { 36, 24,  8,  8 },
                    .lifetime = 384
                };
                arrput(explosions, player_shot_explosion);
                arrdel(player.shots, j);
                return;
            }
        }
    }
}

void process_horde_collision_with_bunkers()
{
    // didn't reach bunkers yet or had already wiped them out
    if (horde.invaders[0].y < 192 || horde.invaders[0].y >= 208)
        return;
    
    for (int b = 0; b < 4; b++)
    {
        for (int i = 0; i < arrlen(horde.invaders); i++)
        {
            SDL_Rect invader_rect = {
                horde.invaders[i].x + 2, horde.invaders[i].y, 8, 8
            };
            if (horde.invaders[i].clip.y == 24)
            {
                invader_rect.x -= 1;
                invader_rect.w = 11;
            }
            else if (horde.invaders[i].clip.y == 32)
            {
                invader_rect.x -= 2;
                invader_rect.w = 12;
            }

            for (int p = 0; p < 352; p++)
            {
                if (point_in_rect(&bunkers[b].points[p], &invader_rect))
                    bunkers[b].points[p].x = -1;
            }
        }
    }
}

void process_player_shot_collision_with_bunker(int b)
{
    for (int i = 0; i < arrlen(player.shots); i++)
    {
        const SDL_Rect shot_rect = {
            player.shots[i].x, player.shots[i].y, 1, 5
        };
        if (SDL_HasIntersection(&bunkers[b].out_rect, &shot_rect))
        {
            for (int p = 0; p < 352; p++)
            {
                if (point_in_rect(&bunkers[b].points[p], &shot_rect))
                {
                    const struct explosion_t explosion = {
                        .x = shot_rect.x - 3,
                        .y = shot_rect.y - 4,
                        .clip = { 36, 24,  8,  8 },
                        .lifetime = 256
                    };
                    arrput(explosions, explosion);
                    arrdel(player.shots, i);
                    i--;

                    desintegrate_bunker_from_point(b, p);
                    return;
                }
            }
        }
    }
}

void process_horde_shot_collision_with_bunker(int b)
{
    for (int i = 0; i < arrlen(horde.shots); i++)
    {
        // middle point of shot rect
        const SDL_Rect shot_rect = {
            horde.shots[i].x + 1, horde.shots[i].y + 1, 1, 1
        };
        if (SDL_HasIntersection(&bunkers[b].out_rect, &shot_rect))
        {
            for (int p = 0; p < 352; p++)
            {
                if (point_in_rect(&bunkers[b].points[p], &shot_rect))
                {
                    const struct explosion_t explosion = {
                        .x = shot_rect.x - 2,
                        .y = shot_rect.y - 4,
                        .clip = { 37, 32,  6,  8 },
                        .lifetime = 256
                    };
                    arrput(explosions, explosion);
                    arrdel(horde.shots, i);
                    i--;

                    desintegrate_bunker_from_point(b, p);
                    return;
                }
            }
        }
    }
}


// base //

void reset_play()
{
    score = 0;

    play.timer = 0;
    play.state = PLAY_PLAYING;

    explosions = NULL;

    player.state = PLAYER_STARTING;
    player.lives = 3;
    player.dying_clip_swaps = 0;
    player.shots = NULL;
    player.timer = 0;

    horde.state = HORDE_STARTING;
    horde.invaders = NULL;
    horde.invaders_updated = 0;
    horde.shots = NULL;
    horde.timer = 0;
    horde.shot_timer = 0;
    horde.shot_timeout = 16 * 32 * (rand() % 2 + 1);

    tourist.state = TOURIST_UNAVAILABLE;
    tourist.available_appearances = rand() % 2 + 2; // 2 or 3 times
    tourist.timer = 0;
    tourist.spawn_timeout = gen_tourist_spawn_timeout();

    for (int b = 0; b < 4; b++)
    {
        bunkers[b].out_rect = (SDL_Rect){ 32 + 46 * b, 192, 22, 16 };

        for (int p = 0; p < 352; p++)
        {
            bunkers[b].points[p] = (SDL_Point){
                p % 22 + 32 + 46 * b, p / 22 + 192
            };
        }

        bunkers[b].points[293] = (SDL_Point){ -1, -1 };
        bunkers[b].points[300] = (SDL_Point){ -1, -1 };

        for (int i = 3; i >= 0; i--)
        {
            for (int j = 0; j < 4 - i; j++)
            {
                bunkers[b].points[22 * i + j] = (SDL_Point){ -1, -1 };
                bunkers[b].points[22 * i - j + 21] = (SDL_Point){ -1, -1 };
            }
        }

        for (int i = 8; i < 14; i++)
        {
            bunkers[b].points[264 + i] = (SDL_Point){ -1, -1 };
            bunkers[b].points[286 + i] = (SDL_Point){ -1, -1 };
        }

        for (int i = 6; i < 16; i++)
        {
            bunkers[b].points[308 + i] = (SDL_Point){ -1, -1 };
            bunkers[b].points[330 + i] = (SDL_Point){ -1, -1 };
        }
    }

    for (int i = 0; i < WIDTH; i++)
        useless_bar[i] = (SDL_Point){ i, 239 };
}

void process_play_events(const SDL_Event* event)
{
    if (event->type == SDL_KEYDOWN && !event->key.repeat &&
        event->key.keysym.sym == SDLK_ESCAPE)
    {
        screen = SCREEN_PAUSE;
        reset_pause();
    }
}

void update_play(Uint32 delta)
{
    switch (play.state)
    {
    case PLAY_PLAYING:
        update_explosions(delta);
        update_player(delta);
        if (player.state != PLAYER_DYING && player.state != PLAYER_DEAD)
        {
            update_tourist(delta);
            update_horde(delta);
        }

        update_player_shots();
        update_horde_shots(delta);

        process_shot_collision_with_player();
        process_shot_collision_with_horde();
        process_shot_collision_with_tourist();
        process_collision_between_shots();
        for (int b = 0; b < 4; b++)
        {
            process_player_shot_collision_with_bunker(b);
            process_horde_shot_collision_with_bunker(b);
        }
        process_horde_collision_with_bunkers();

        // update hi-score
        if (hi_score < score)
            hi_score = score;

        // all invaders killed
        if (player.state == PLAYER_ALIVE && arrlen(horde.invaders) == 0)
        {
            play.state = PLAY_RESTARTING;
            play.timer = 0;
            arrfree(horde.shots);
            arrfree(player.shots);
        }

        if (player.lives == 0 && player.state == PLAYER_DEAD)
        {
            screen = SCREEN_GAMEOVER;
            reset_over();
        }
        break;
    case PLAY_RESTARTING:
        update_explosions(delta);
        
        play.timer += delta;
        if (play.timer >= 1504)
        {
            // keep score and life counter
            const int score_backup = score;
            const int lives_backup = player.lives;
            reset_play();
            score = score_backup;
            player.lives = lives_backup;
        }
        break;
    }
}

void render_play()
{
    // Just to resemble the original game
    SDL_SetRenderDrawColor(ren, 32, 255, 32, 255);
    SDL_RenderDrawPoints(ren, useless_bar, WIDTH);

    // all gameplay elements
    render_player_shots();
    render_horde_shots();
    render_tourist();
    render_horde();
    render_player();
    render_bunkers();
    render_explosions();

    // live counter (number)
    char player_lives_text[3];
    sprintf(player_lives_text, "%d", player.lives);
    render_text(player_lives_text, 2, 8, HEIGHT - 16);
    // live counter (cannons)
    const SDL_Rect cannon_clip = { 0, 8, 16, 8 };
    for (int i = 0; i < player.lives - 1; i++)
        render_clip(&cannon_clip, 24 + i * 16, HEIGHT - 16);
}


void process_screen_events(const SDL_Event* event)
{
    process_credit_events(event);

    if (event->type == SDL_QUIT)
        screen = SCREEN_EXIT;
    else switch (screen)
    {
    case SCREEN_MENU:
        process_menu_events(event);
        break;
    case SCREEN_PLAY:
        process_play_events(event);
        break;
    case SCREEN_PAUSE:
        process_pause_events(event);
        break;
    case SCREEN_GAMEOVER:
        process_over_events(event);
        break;
    }
}

void update_and_render_screen(Uint32 delta)
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    switch (screen)
    {
    case SCREEN_PAUSE:
        update_pause(delta);
        render_play();
        render_pause();
        break;
    case SCREEN_MENU:
        update_menu(delta);
        render_menu();
        break;
    case SCREEN_PLAY:
        update_play(delta);
        render_play();
        break;
    case SCREEN_GAMEOVER:
        update_over(delta);
        render_play();
        render_over();
        break;
    }
    render_scores();
    render_credits();

    SDL_RenderPresent(ren);
}

///////////////////////////////////////////////////////////////////////////////
// RESOURCE MANAGEMENT //
///////////////////////////////////////////////////////////////////////////////

void load_resources()
{
    score = hi_score = 0;
    FILE* hi_score_file = fopen("hi_score.txt", "r");
    if (hi_score_file)
    {
        fscanf(hi_score_file, "%6d", &hi_score);
        fclose(hi_score_file);
    }
    else
        fclose(fopen("hi_score.txt", "w"));

    atlas = IMG_LoadTexture(ren, RESOURCES "/atlas.png");
    SDL_assert(atlas);
}

void free_resources()
{
    SDL_DestroyTexture(atlas);

    FILE* hi_score_file = fopen("hi_score.txt", "w");
    fprintf(hi_score_file, "%6d", hi_score);
    fclose(hi_score_file);
}

///////////////////////////////////////////////////////////////////////////////
// MAIN LOOP and ENTRY POINT //
///////////////////////////////////////////////////////////////////////////////

void main_loop()
{
    Uint32 event_start = 0, event_wait = 1000 / FPS;
    Uint32 frame_start = 0, frame_delta = 0;

    while (screen != SCREEN_EXIT)
    {
        SDL_Event event;
        if (SDL_WaitEventTimeout(&event, event_wait))
        {
            process_screen_events(&event);

            const uint32_t event_delta = SDL_GetTicks() - event_start;
            event_start += event_delta;
            // wait less next time
            event_wait -= event_delta < event_wait ? event_delta : event_wait;
        }
        else
        {
            update_and_render_screen(frame_delta);
        
            frame_delta = SDL_GetTicks() - frame_start;
            frame_start += frame_delta;
            // reset event timing
            event_start = frame_start;
            event_wait = 1000 / FPS;
        }
    }
}

int main(int argc, char** args)
{
    // INITIALIZATION //

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    srand(time(NULL));

    SDL_ShowCursor(SDL_DISABLE);

    screen = SCREEN_MENU;
    reset_menu();
    win = SDL_CreateWindow("Space Invaders Clone", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCALE * WIDTH, SCALE * HEIGHT,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(ren, SCALE, SCALE);
    SDL_RenderSetIntegerScale(ren, SDL_TRUE);

    load_resources();
    
    main_loop(); // EXECUTION //

    // TERMINATION //

    free_resources();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    IMG_Quit();
    SDL_Quit();

    arrfree(horde.invaders);
    arrfree(horde.shots);
    arrfree(player.shots);
    arrfree(explosions);

    return 0;
}
