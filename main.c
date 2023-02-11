#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#undef STB_DS_IMPLEMENTATION

// resources
SDL_Texture* atlas = NULL, * font_atlas = NULL;

/* APP */

#define FPS 60
#define SCALE 2
#define RESOURCE_DIR "./res"
#define FONT_PTSIZE 8

#define CHARACTERS_MONO " A B C D E F G H  I J K L M N O P Q R S T U V W X"\
	" Y Z 0  1 2 3 4 5 6 7 8 9 * ? -  < > = "
#define CHARACTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?-<>="

#define WORLD_WIDTH  224
#define WORLD_HEIGHT 256

struct {
	enum {
        APP_MENU,
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
        int index = 0;
        for (; index < len_characters; index++)
            if (toupper(text[i]) == characters[index]) break;

        if (index != len_characters) // success
        {
            SDL_Rect clip = {
                index * FONT_PTSIZE, 0, FONT_PTSIZE, FONT_PTSIZE
            };
            SDL_Rect scale = {
                SCALE * x, SCALE * y, SCALE * clip.w, SCALE * clip.h
            };

            // Correcting placement of 'I', '1' and '='. They are thinner.
            if (toupper(text[i]) == 'I' || text[i] == '1' || text[i] == '=')
                scale.x -= SCALE;

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
    char credits_str[10];
    sprintf(credits_str, "CREDIT %02d", app.credits);
    render_text(credits_str, WORLD_WIDTH - 80, WORLD_HEIGHT - 16);
}


void render_scores()
{
    char score_str[7];
    // score
    render_text("your score", 8, 8);
    sprintf(score_str, "%06d", app.score);
    render_text(score_str, 24, 24);
    // high-score
    render_text("high-score", WORLD_WIDTH - 88, 8);
    sprintf(score_str, "%06d", app.hi_score);
    render_text(score_str, WORLD_WIDTH - 72, 24);
}

void render_score_advances_table()
{
    render_text("*SCORE ADVANCES TABLE*", 24, 128);

    const SDL_Rect tourist_clip = { 0,  0, 24,  8 };
    render_clip(&tourist_clip, 58, 152);
    render_text("=?? MYSTERY", 80, 152);

    const SDL_Rect invader1_clip = { 0, 16, 12,  8 };
    render_clip(&invader1_clip, 64, 168);
    render_text("=30 POINTS", 80, 168);

    const SDL_Rect invader2_clip = { 0, 24, 12,  8 };
    render_clip(&invader2_clip, 64, 184);
    render_text("=20 POINTS", 80, 184);

    const SDL_Rect invader3_clip = { 0, 32, 12,  8 };
    render_clip(&invader3_clip, 64, 200);
    render_text("=10 POINTS", 80, 200);
}


/* SCREENS STRUCTS (for each app screen) */

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

    uint32_t timer, wait_timer, shot_timeout;
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


/* MENU SCREEN */

void reset_menu()
{
    menu.state = MENU_DISPLAYING;
    menu.display_i = 0;
    menu.timer = 0;
}

void reset_play();
void process_menu_events()
{
    if (app.event.type != SDL_KEYDOWN || app.event.key.repeat)
        return;
    
    SDL_Keycode key = app.event.key.keysym.sym;
    switch (menu.state)
    {
    case MENU_DISPLAYING:
    case MENU_WAITING:
        if (key == SDLK_RETURN || key == SDLK_RETURN2)
        {
            menu.state = MENU_BLINKING_ON;
            menu.timer = 0;
        }
        break;
    case MENU_BLINKING_ON:
    case MENU_BLINKING_OFF:
        if (key == SDLK_p)
        {
            app.screen = APP_PLAY;
            if (app.credits > 0)
                app.credits--;
            reset_play();
        }
        else if (key == SDLK_q)
            app.screen = APP_QUIT;
        break;
    }
}

void update_menu()
{
    switch (menu.state)
    {
    case MENU_DISPLAYING:
        menu.timer += app.frame_time;
        if (menu.timer >= 160)
        {
            if (++menu.display_i == 14)
                menu.state = MENU_WAITING;
            menu.timer = 0;
        }
        break;
    case MENU_WAITING:
        menu.timer += app.frame_time;
        if (menu.timer >= 1008)
        {
            menu.state = MENU_BLINKING_ON;
            menu.timer = 0;
        }
        break;
    case MENU_BLINKING_ON:
    case MENU_BLINKING_OFF:
        menu.timer += app.frame_time;
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
    render_scores();
    render_credits();

    switch (menu.state)
    {
    case MENU_BLINKING_ON:
        render_text("<P> PLAY", 80, 80);
        render_text("<Q> QUIT", 80, 96);
    case MENU_BLINKING_OFF:
        render_text("SPACE INVADERS", 56, 56);
        render_score_advances_table();
        break;
    case MENU_WAITING:
    case MENU_DISPLAYING:
        render_text_until("SPACE INVADERS", 56, 56, menu.display_i);
        break;
    }
}


/* GAME OVER SCREEN */

void reset_over()
{
    over.state = GAMEOVER_WAITING1;
    over.display_i = 0;
    over.timer = 0;
}

void reset_play();
void process_over_events()
{
    if (app.event.type != SDL_KEYDOWN || app.event.key.repeat)
        return;

    SDL_Keycode key = app.event.key.keysym.sym;
    switch (over.state)
    {
    case GAMEOVER_WAITING1:
        if (key == SDLK_RETURN || key == SDLK_RETURN2)
        {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_WAITING2:
    case GAMEOVER_DISPLAYING:
        if (key == SDLK_RETURN || key == SDLK_RETURN2)
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
            app.screen = APP_PLAY;
            if (app.credits > 0)
                app.credits--;
            reset_play();
            break;
        case SDLK_q:
            app.screen = APP_QUIT;
            break;
        case SDLK_m:
            app.screen = APP_MENU;
            reset_menu();
            break;
        }
        break;
    }
}

void update_over()
{
    switch (over.state)
    {
    case GAMEOVER_WAITING1:
        over.timer += app.frame_time;
        if (over.timer >= 2000)
        {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_DISPLAYING:
        over.timer += app.frame_time;
        if (over.timer >= 160)
        {
            if (++over.display_i == 8)
                over.state = GAMEOVER_WAITING2;
            over.timer = 0;
        }
        break;
    case GAMEOVER_WAITING2:
        over.timer += app.frame_time;
        if (over.timer >= 1008)
        {
            over.state = GAMEOVER_BLINKING_ON;
            over.timer = 0;
        }
        break;
    case GAMEOVER_BLINKING_ON:
    case GAMEOVER_BLINKING_OFF:
        over.timer += app.frame_time;
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
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 225);
        const SDL_Rect overlay_rect = {
            0, 0, SCALE * WORLD_WIDTH, SCALE * WORLD_HEIGHT
        };
        SDL_RenderFillRect(app.renderer, &overlay_rect);
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
    }

    switch (over.state)
    {
    case GAMEOVER_WAITING2:
    case GAMEOVER_DISPLAYING:
        SDL_SetTextureColorMod(font_atlas, 216, 32, 32);
        render_text_until("YOU LOST", 80, 56, over.display_i);
        SDL_SetTextureColorMod(font_atlas, 255, 255, 255);
        break;
    case GAMEOVER_BLINKING_ON:
        render_text("<ENTER> GO AGAIN", 48, 80);
        render_text("<M> MENU", 80, 96);
        render_text("<Q> QUIT", 80, 112);
    case GAMEOVER_BLINKING_OFF:
        SDL_SetTextureColorMod(font_atlas, 216, 32, 32);
        render_text("YOU LOST", 80, 56);
        SDL_SetTextureColorMod(font_atlas, 255, 255, 255);
        break;
    }
}


/* PAUSE SCREEN */

void reset_pause()
{
    pause.state = PAUSE_BLINKING_ON;
    pause.timer = 0;
}

void process_pause_events()
{
    if (app.event.type != SDL_KEYDOWN || app.event.key.repeat ||
        pause.state == PAUSE_RESUMING)
        return;
    
    switch (app.event.key.keysym.sym)
    {
    case SDLK_m:
        app.screen = APP_MENU;
        reset_menu();
        break;
    case SDLK_ESCAPE:
        pause.state = PAUSE_RESUMING;
        pause.timer = 0;
        break;
    case SDLK_q:
        app.screen = APP_QUIT;
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
        if (pause.timer >= 498) // 16 * 32 (half second)
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
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 225);
    const SDL_Rect overlay_rect = {
        0, 0, SCALE * WORLD_WIDTH, SCALE * WORLD_HEIGHT
    };
    SDL_RenderFillRect(app.renderer, &overlay_rect);
    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);

    switch (pause.state)
    {
    case PAUSE_BLINKING_ON:
        render_text("<ESC> RESUME", 64, 64);
        render_text("<M> MENU", 80, 80);
        render_text("<Q> QUIT", 80, 96);
    case PAUSE_BLINKING_OFF:
        render_score_advances_table();
        break;
    case PAUSE_RESUMING: {
        int countdown = 3 - (int)pause.timer / 1000;
        char countdown_str[3];
        sprintf(countdown_str, "%02d", countdown == 0 ? 1 : countdown);
        render_text(countdown_str, WORLD_WIDTH / 2 - 8, 112);
        break; }
    }
}


/* PLAY SCREEN */

void update_explosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        explosions[i].timer += app.frame_time;
        if (explosions[i].timer >= explosions[i].timeout)
        {
            arrdel(explosions, i);
            i--;
        }
    }
}

void render_explosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
        render_clip(
            &explosions[i].clip, explosions[i].x, explosions[i].y
        );
}


void update_player()
{
	switch (player.state)
	{
    case PLAYER_STARTING:
        player.timer += app.frame_time;
        if (player.timer >= 2000)
        {
            player.state = PLAYER_ALIVE;
            player.x = 14;
            player.timer = 0;
        }
        break;
	case PLAYER_ALIVE: {
        const uint8_t* keys = SDL_GetKeyboardState(NULL);
        // moving player //
        if (keys[SDL_SCANCODE_LEFT] && player.x > 14)
            player.x -= 1;
        if (keys[SDL_SCANCODE_RIGHT] && player.x < WORLD_WIDTH - 31)
            player.x += 1;

        // shooting mechanic //
        if (player.timer < 768)
            player.timer += app.frame_time;
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
        player.timer += app.frame_time;
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
		player.timer += app.frame_time;
		if (player.timer >= 2000)
		{
            // horde reached player. game over
            if (horde.invaders[0].y == 216)
            {
                app.screen = APP_GAMEOVER;
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
                .timer = 0,
                .timeout = 16 * 16
            };
            arrput(explosions, explosion);
            arrdel(player.shots, i);
            i--;
        }
	}
}

void render_player_shots()
{
    for (int i = 0; i < arrlen(player.shots); i++)
	{
		SDL_Rect rect = {
			SCALE * player.shots[i].x,
			SCALE * player.shots[i].y,
			SCALE,
			SCALE * 4,
		};
		SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(app.renderer, &rect);
	}
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
        horde.shot_timeout = 498 * (rand() % 2 + 1);
    }
}

void make_horde_shoot()
{
    if (horde.timer < horde.shot_timeout || arrlen(horde.invaders) == 0)
        return;

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
    arrput(horde.shots, shot);

    // reset
    horde.timer = 0;
    horde.shot_timeout = 498 * (rand() % 2 + 1);
}

void move_horde()
{
    if (arrlen(horde.invaders) == 0)
        return;

    horde.invaders_updated %= arrlen(horde.invaders);
    
    const int i = horde.invaders_updated;
    // move this guy
    horde.invaders[i].x += horde.xmove;
    horde.invaders[i].y += horde.ymove;
    // animate
    horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0;
    // now someone got updated this frame
    horde.invaders_updated++;
    
    // horde entirely updated so maybe it's time to flip directions next frame
    bool all_updated = horde.invaders_updated == arrlen(horde.invaders);
    if (all_updated && horde.ymove == 0)
    {
        // find out if horde should change direction
        for (int j = 0; j < arrlen(horde.invaders); j++)
        {
            const int x = horde.invaders[j].x;
            if (x <= 10 || x >= WORLD_WIDTH - 22) // should change directions
            {
                horde.xmove = -horde.xmove;
                horde.ymove = 8;
                break;
            }
        }
    }
    else if (all_updated)
        horde.ymove = 0;
}

void update_horde()
{
    switch (horde.state)
    {
    case HORDE_STARTING:
        update_horde_start_anim();
        break;
    case HORDE_MOVING:
        if (player.state == PLAYER_ALIVE)
        {
            horde.timer += app.frame_time;
            make_horde_shoot();           
        }
        if (player.state == PLAYER_STARTING || player.state == PLAYER_ALIVE)
            move_horde();
        break;
    case HORDE_WAITING:
        horde.wait_timer += app.frame_time;
        if (horde.wait_timer >= 256)
            horde.state = HORDE_MOVING;
        break;
    }
}

void update_horde_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
	{
        // update shot animation
        horde.shots[i].timer += app.frame_time;
        if (horde.shots[i].timer >= 16 * 6)
        {
            horde.shots[i].clip.x -= 24;
            horde.shots[i].clip.x = (horde.shots[i].clip.x + 3) % 12;
            horde.shots[i].clip.x += 24;
            horde.shots[i].timer = 0;
        }

        // reached bottom of screen
		horde.shots[i].y++;
		if (horde.shots[i].y >= 232)
        {
            // add explosion
            const struct explosion_t explosion = {
                .x = horde.shots[i].x - 1,
                .y = 232,
                .clip = { 24, 40,  6,  8 },
                .timer = 0,
                .timeout = 16 * 8
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
        render_clip(
            &horde.invaders[i].clip,
            horde.invaders[i].x,
            horde.invaders[i].y
        );
}

void render_horde_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
        render_clip(
            &horde.shots[i].clip,
            horde.shots[i].x,
            horde.shots[i].y
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
    switch (tourist.state)
    {
    case TOURIST_AVAILABLE:
        tourist.x += tourist.xvel;
        // reached end of screen. unavailable
        if (tourist.x <= 8.f || tourist.x >= WORLD_WIDTH - 32.f)
            tourist.state = TOURIST_UNAVAILABLE;
        break;
    case TOURIST_UNAVAILABLE:
        if (tourist.available_appearances == 0)
            break;

        tourist.timer += app.frame_time;
        if (tourist.timer >= tourist.spawn_timeout) // spawn
        {
            tourist.state = TOURIST_AVAILABLE;
            // spawn either left or right
            tourist.xvel = rand() % 2 ? 0.65f : -0.65f;
            tourist.x = tourist.xvel > 0.0f ? 8.f : (WORLD_WIDTH - 32.f);

            tourist.score_inc = gen_tourist_score();
 
            tourist.available_appearances--;

            tourist.timer = 0; // reset timer
            tourist.spawn_timeout = gen_tourist_spawn_timeout();
        }
        break;
    case TOURIST_DYING:
        tourist.timer += app.frame_time;
        if (tourist.timer >= 384)
        {
            tourist.state = TOURIST_SHOWING_SCORE;
            app.score += tourist.score_inc;
            tourist.timer = 0; // reset timer
        }
        break;
    case TOURIST_SHOWING_SCORE:
        tourist.timer += app.frame_time;
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
        sprintf(tourist_score, "%d", tourist.score_inc);
        SDL_SetTextureColorMod(font_atlas, 216, 32, 32);
        render_text(tourist_score, tourist.x, 40);
        SDL_SetTextureColorMod(font_atlas, 255, 255, 255);
        break; }
    }
}


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
            tourist.state = TOURIST_DYING;
            player.lives += rand() % 2;
            
            arrdel(player.shots, i);
            i--;
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
        if (SDL_HasIntersection(&player_rect, &shot_rect))
        {
            arrdel(horde.shots, i);
            i--;
            // player dead now
            player.state = PLAYER_DYING;
            player.timer = 0;
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
                // add explosion
                const struct explosion_t explosion = {
                    .x = horde.invaders[j].x,
                    .y = horde.invaders[j].y,
                    .clip = { 0, 40, 13,  8 },
                    .timer = 0,
                    .timeout = 256
                };
                arrput(explosions, explosion);
                arrdel(player.shots, i);
                i--;

                // prevent updating twice the same invader (corner case)
                if (j < horde.invaders_updated)
                    horde.invaders_updated--;
                
                arrdel(horde.invaders, j);
                app.score += score_inc;
                // horde now needs to wait (make delay effect)
                horde.state = HORDE_WAITING;
                horde.wait_timer = 0;
                break;
            }
        }
    }
}

void process_collision_between_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
    {
        const SDL_Rect horde_shot_rect = {
            horde.shots[i].x, horde.shots[i].y, 3, 4
        };
        for (int j = 0; j < arrlen(player.shots); j++) 
        {
            const SDL_Rect player_shot_rect = {
                player.shots[i].x, player.shots[i].y - 4, 1, 4
            };
            if (SDL_HasIntersection(&horde_shot_rect, &player_shot_rect))
            {
                // horde shot may cut right through player shots (it's stronger)
                if (rand() % 4 != 0)
                {
                    const struct explosion_t horde_shot_explosion = {
                        .x = horde_shot_rect.x - 2,
                        .y = horde_shot_rect.y,
                        .clip = { 24, 40,  6,  8 },
                        .timer = 0,
                        .timeout = 16 * 24
                    };
                    arrput(explosions, horde_shot_explosion);
                    arrdel(horde.shots, i);
                    i--;
                }
                const struct explosion_t player_shot_explosion = {
                    .x = player_shot_rect.x - 3,
                    .y = player_shot_rect.y,
                    .clip = { 36, 24,  8,  8 },
                    .timer = 0,
                    .timeout = 16 * 24
                };
                arrput(explosions, player_shot_explosion);
                arrdel(player.shots, j);
                break;
            }
        }
    }
}


void reset_play()
{
    app.score = 0;

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
    horde.wait_timer = 0;
    horde.shot_timeout = 16 * 32 * (rand() % 2 + 1);

    tourist.state = TOURIST_UNAVAILABLE;
    tourist.available_appearances = rand() % 2 + 2; // 2 or 3 times
    tourist.timer = 0;
    tourist.spawn_timeout = gen_tourist_spawn_timeout();
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
        if (player.state != PLAYER_DYING && player.state != PLAYER_DEAD)
        {
            update_tourist();
            update_horde();
        }

        update_player_shots();
        update_horde_shots();

        process_shot_collision_with_player();
        process_shot_collision_with_horde();
        process_shot_collision_with_tourist();
        process_collision_between_shots();

        if (player.state == PLAYER_ALIVE && arrlen(horde.invaders) == 0)
        {
            play.state = PLAY_RESTARTING;
            play.timer = 0;
            arrfree(horde.shots);
            arrfree(player.shots);
        }

        // update score
        if (app.hi_score < app.score)
            app.hi_score = app.score;

        if (player.lives == 0 && player.state == PLAYER_DEAD)
        {
            app.screen = APP_GAMEOVER;
            reset_over();
        }
        break;
    case PLAY_RESTARTING:
        update_explosions();
        
        play.timer += app.frame_time;
        if (play.timer >= 1504)
        {
            const int tmp_score = app.score;
            reset_play();
            app.score = tmp_score;
        }
        break;
    }
}

void render_play()
{
    // useless stuff
    // bar. Just to resemble the original game
    SDL_SetRenderDrawColor(app.renderer, 32, 255, 32, 255); // #20ff20
    const SDL_Rect rect = {
        SCALE * 0, SCALE * 239, SCALE * WORLD_WIDTH, SCALE
    };
    SDL_RenderFillRect(app.renderer, &rect);

    render_player_shots();
    render_horde_shots();
    render_tourist();
    render_horde();
    render_player();
    render_explosions();

    // live counter
    char player_lives_str[2] = { '0' + player.lives, '\0' };
    render_text(player_lives_str, 8, WORLD_HEIGHT - 16);
    // live cannons
    const SDL_Rect cannon_clip = { 0, 8, 16, 8 };
    for (int i = 0; i < player.lives - 1; i++)
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
            case APP_MENU:
                process_menu_events();
                break;
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
            SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
            SDL_RenderClear(app.renderer);
            switch (app.screen)
            {
            case APP_PAUSE:
                update_pause();
                render_play();
                render_pause();
                break;
            case APP_MENU:
                update_menu();
                render_menu();
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

    app.screen = APP_MENU;
    reset_menu();
    app.window = SDL_CreateWindow(
        "Space Invaders Clone",
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
        fscanf(hi_score_file, "%6d", &app.hi_score);
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
    fprintf(hi_score_file, "%6d", app.hi_score);
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

// 22 x 16
struct {
    SDL_Rect outer_bounds;
    SDL_Point points[352];
} bunkers[4];

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
