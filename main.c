#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "stb_ds.h"

// resources
SDL_Texture* atlas = NULL, * font_atlas = NULL;

/* APP */

#define APP_FPS 60
#define APP_SCALE 2
#define APP_RESOURCE_DIR "./res"
#define APP_FONT_PTSIZE 8

#define APP_CHARACTERS_MONO " A B C D E F G H  I J K L M N O P Q R S T U V W X"\
	" Y Z 0  1 2 3 4 5 6 7 8 9 - ? <  > = "
#define APP_CHARACTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-?<>="

#define WORLD_WIDTH  224
#define WORLD_HEIGHT 264

struct {
    enum {
        APP_IN_MENU,
        APP_IN_GAME,
        APP_QUITTING
    } state;

    SDL_Window* window;
    SDL_Event event;

    SDL_Renderer* renderer;

    uint32_t frame_time;
} app;

// utilities

void app_render_text(const char* text, int x, int y)
{
    static const char* characters = APP_CHARACTERS;
    for (int i = 0; i < strlen(text); i++)
    {
        // find mapping
        int mapping = 0;
        for (; mapping < strlen(characters); mapping++)
            if (toupper(text[i]) == characters[mapping]) break;

        if (mapping != strlen(characters)) // success
        {
            SDL_Rect clip = {
                mapping * APP_FONT_PTSIZE,
                0,
                APP_FONT_PTSIZE,
                APP_FONT_PTSIZE
            };
            SDL_Rect scale = {
                APP_SCALE * x,
                APP_SCALE * y,
                APP_SCALE * APP_FONT_PTSIZE,
                APP_SCALE * APP_FONT_PTSIZE
            };

            // Correcting placement of quirky characters
            // I, 1 and = are thinner.
            if (toupper(text[i]) == 'I' || text[i] == '1' || text[i] == '=')
                scale.x -= APP_SCALE;
            // A is just a litle bit off to the right.
            else if (toupper(text[i]) == 'A')
                scale.x += APP_SCALE;

            SDL_RenderCopy(app.renderer, font_atlas, &clip, &scale);
        }

        x += APP_FONT_PTSIZE;
    }
}

void app_render_clip(const SDL_Rect* clip, int x, int y)
{
    SDL_Rect scale = {
        APP_SCALE * x,
        APP_SCALE * y,
        APP_SCALE * clip->w,
        APP_SCALE * clip->h
    };
    SDL_RenderCopy(app.renderer, atlas, clip, &scale);
}

/* GAME STATE */

enum {
    GAME_STARTING,
    GAME_RESTARTING,
    GAME_PAUSED,
    GAME_PLAYING,
    GAME_OVER
} game_state;

int game_score;
int game_hi_score;

void render_scores()
{
    char format[10];
    // score
    app_render_text("score", 8, 8);
    sprintf(format, "%05d", game_score);
    app_render_text(format, 8, 24);
    // high-score
    app_render_text("hi-score", WORLD_WIDTH - 72, 8);
    sprintf(format, "%05d", game_hi_score);
    app_render_text(format, WORLD_WIDTH - 48, 24);
}

// explosions

struct explosion_t {
    int x, y;
    SDL_Rect clip;
    uint32_t timing, timeout;
}* explosions;

void update_explosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        explosions[i].timing += app.frame_time;
        if (explosions[i].timing >= explosions[i].timeout)
            arrdel(explosions, i);
    }
}

// horde

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 72

#define HORDE_SIZE 55

struct {
    enum {
        HORDE_STARTING, // start animation
        HORDE_MOVING_LEFT,
        HORDE_MOVING_DOWN_LEFT,
        HORDE_MOVING_RIGHT,
        HORDE_MOVING_DOWN_RIGHT
    } state;

    struct invader_t {
        SDL_Rect clip;
        int x, y;
    }* invaders;

    struct horde_shot_t {
        int x, y;
        SDL_Rect clip;
        uint32_t anim_timing;
    }* shots;

    // One invader is updated each frame.
    // horde is up-to-date only when all invaders are up-to-date.
    int invaders_updated;

    uint32_t shot_timing, shot_timeout;
} horde;

void play_horde_start_anim()
{
    if (horde.invaders_updated < HORDE_SIZE)
    {
        const int row = 4 - horde.invaders_updated / 11; // 4, 3, 2, 1, 0
        const int col = horde.invaders_updated % 11; // 0, 1, 2, ..., 10
        // clip rect for invader
        SDL_Rect clip = {12, 16, 12, 8};
        if (row > 2) clip.y = 32; // in 4th or 5th row
        else if (row > 0) clip.y = 24; // in 2nd or 3rd row
        const struct invader_t invader = {
            .clip = clip,
            .x = HORDE_X_INIT + 16 * col,
            .y = HORDE_Y_INIT + 16 * row
        };
        arrput(horde.invaders, invader);
        horde.invaders_updated++;
    }
    else // done. Now move right
    {
        horde.state = HORDE_MOVING_RIGHT;
        horde.invaders_updated = 0;
    }
}

void move_horde_horizontally()
{
    if (arrlen(horde.invaders) == 0)
        return;
    
    const int i = horde.invaders_updated;
    // move this guy
    horde.invaders[i].x += horde.state == HORDE_MOVING_LEFT ? -2 : 2;
    horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0;
    // now someone got updated
    horde.invaders_updated++;
    
    // horde updated and now it's time to flip directions
    if (horde.invaders_updated == arrlen(horde.invaders))
    {
        // find out if horde should change direction
        for (int i = 0; i < arrlen(horde.invaders); i++)
        {
            const int x = horde.invaders[i].x;
            if (x <= 10 || x >= WORLD_WIDTH - 22) // should change directions
            {
                horde.state = horde.state == HORDE_MOVING_LEFT ?
                    HORDE_MOVING_DOWN_RIGHT : HORDE_MOVING_DOWN_LEFT;
                break;
            }
        }
        
        horde.invaders_updated = 0; // no one updated
    }
}

void move_horde_diagonally()
{
    if (arrlen(horde.invaders) == 0)
        return;

    const int i = horde.invaders_updated;
    horde.invaders[i].x += horde.state == HORDE_MOVING_DOWN_LEFT ? -2 : 2;
    horde.invaders[i].y += 8;
    horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0;

    horde.invaders_updated++;
    
    if (horde.invaders_updated == arrlen(horde.invaders))
    {
        horde.state = horde.state == HORDE_MOVING_DOWN_LEFT ?
            HORDE_MOVING_LEFT : HORDE_MOVING_RIGHT;
        horde.invaders_updated = 0;
    }
}

void update_horde_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
	{
        // update shot animation
        horde.shots[i].anim_timing += app.frame_time;
        if (horde.shots[i].anim_timing >= 16 * 6)
        {
            horde.shots[i].clip.x -= 24;
            horde.shots[i].clip.x = (horde.shots[i].clip.x + 3) % 12;
            horde.shots[i].clip.x += 24;
            horde.shots[i].anim_timing = 0;
        }

		horde.shots[i].y++;
        // reached bottom of screen
		if (horde.shots[i].y >= 240)
        {
            // add explosion
            const struct explosion_t explosion = {
                .x = horde.shots[i].x - 1,
                .y = 240,
                .clip = { 24, 40,  6,  8 },
                .timing = 0,
                .timeout = 16 * 24
            };
            arrput(explosions, explosion);
			arrdel(horde.shots, i);
        }
	}
}

void make_horde_shoot()
{
    horde.shot_timing += app.frame_time;
    if (horde.shot_timing >= horde.shot_timeout && arrlen(horde.invaders) > 0)
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
            .anim_timing = 0
        };
        arrput(horde.shots, shot);

        // reset
        horde.shot_timing = 0;
        horde.shot_timeout = 1024 * (rand() % 2 + 1);
    }
}

void update_horde()
{
    switch (horde.state)
    {
    case HORDE_STARTING:
        play_horde_start_anim();
        break;
    case HORDE_MOVING_LEFT:
    case HORDE_MOVING_RIGHT:
        make_horde_shoot();
        move_horde_horizontally();
        break;
    case HORDE_MOVING_DOWN_LEFT:
    case HORDE_MOVING_DOWN_RIGHT:
        make_horde_shoot();
        move_horde_diagonally();
        break;
    }
}

void render_horde_shots()
{
    for (int i = 0; i < arrlen(horde.shots); i++)
        app_render_clip(&horde.shots[i].clip, horde.shots[i].x, horde.shots[i].y);
}

void render_horde()
{
    for (int i = 0; i < arrlen(horde.invaders); i++)
    {
        app_render_clip(
            &horde.invaders[i].clip, horde.invaders[i].x, horde.invaders[i].y
        );
    }
}

// tourist

#define TOURIST_Y 48

#define TOURIST_VEL 0.65f

#define TOURIST_DEATH_TIMEOUT (16 * 64)
#define TOURIST_SCORE_TIMEOUT (16 * 64)
#define TOURIST_SPAWN_TIMEOUT_MIN 20 /* seconds */
#define TOURIST_SPAWN_TIMEOUT_MAX 30 /* seconds */

struct {
    enum {
        TOURIST_UNAVAILABLE,
        TOURIST_DYING,
        TOURIST_SHOWING_SCORE,
        TOURIST_AVAILABLE_LEFT,
        TOURIST_AVAILABLE_RIGHT
    } state;

    float x;
    int score_value;
    uint32_t spawn_timing, spawn_timeout;
    uint32_t death_timing, score_timing;
} tourist;

void update_tourist()
{
    switch (tourist.state)
    {
    case TOURIST_AVAILABLE_LEFT:
    case TOURIST_AVAILABLE_RIGHT:
        tourist.x += tourist.state == TOURIST_AVAILABLE_LEFT ?
            TOURIST_VEL : -TOURIST_VEL;
        // reached end of screen. unavailable
        if (tourist.x <= 8.f || tourist.x >= WORLD_WIDTH - 32.f)
        {
            tourist.state = TOURIST_UNAVAILABLE;
            tourist.x = 1000.f; // make sure it doesn't get shot when unavailable
            // give it a random score value for next appearance
            tourist.score_value = 10 * (rand() % 30 + 1);
            // set spawn timer
            tourist.spawn_timing = 0;
            tourist.spawn_timeout = 1024 * (rand() % (TOURIST_SPAWN_TIMEOUT_MAX - \
                TOURIST_SPAWN_TIMEOUT_MIN + 1) + TOURIST_SPAWN_TIMEOUT_MIN);
        }
        break;
    case TOURIST_UNAVAILABLE:
        tourist.spawn_timing += app.frame_time;
        if (tourist.spawn_timing >= tourist.spawn_timeout) // spawn
        {
            tourist.state = rand() % 2 ?
                TOURIST_AVAILABLE_LEFT : TOURIST_AVAILABLE_RIGHT;
            tourist.x = tourist.state == TOURIST_AVAILABLE_LEFT ?
                8.f : (WORLD_WIDTH - 32.f);
        }
        break;
    case TOURIST_DYING:
        tourist.death_timing += app.frame_time;
        if (tourist.death_timing >= TOURIST_DEATH_TIMEOUT)
        {
            tourist.death_timing = 0;
            tourist.state = TOURIST_SHOWING_SCORE;
        }
        break;
    case TOURIST_SHOWING_SCORE:
        tourist.score_timing += app.frame_time;
        if (tourist.score_timing >= TOURIST_SCORE_TIMEOUT)
        {
            tourist.state = TOURIST_UNAVAILABLE;
            tourist.x = 1000.f; // make sure it doesn't get shot when unavailable
            // give it a random score value for next appearance
            tourist.score_value = 10 * (rand() % 30 + 1);
            // set spawn timer
            tourist.spawn_timing = 0;
            tourist.spawn_timeout = 1024 * (rand() % (TOURIST_SPAWN_TIMEOUT_MAX - \
                TOURIST_SPAWN_TIMEOUT_MIN + 1) + TOURIST_SPAWN_TIMEOUT_MIN);
            
            tourist.score_timing = 0;
        }
        break;
    }
}

void render_tourist()
{
    switch (tourist.state)
    {
    case TOURIST_UNAVAILABLE:
        break;
    case TOURIST_DYING:
        const SDL_Rect tourist_rect = {  24,  0, 24,  8 };
        app_render_clip(&tourist_rect, tourist.x, TOURIST_Y);
        break;
    case TOURIST_SHOWING_SCORE:
        char score_str[4];
        sprintf(score_str, "%d", tourist.score_value);
        app_render_text(score_str, tourist.x, TOURIST_Y);
        break;
    default:
        const SDL_Rect clip = {  0,  0, 24,  8 };
        app_render_clip(&clip, tourist.x, TOURIST_Y);
    }
}

// player

#define PLAYER_Y 224
#define PLAYER_SHOT_TIMEOUT  (16 * 48) /* 1 second */
#define PLAYER_DEATH_TIMEOUT (16 * 125) /* 2 seconds */

struct {
    enum {
        PLAYER_ALIVE = 0,
        PLAYER_DEAD
    } state;

    int x;
    int lives;

    SDL_Point* shots;

    // timeouts are constant
    uint32_t shot_timing, death_timing;
} player;

void update_player_shots()
{
    for (int i = 0; i < arrlen(player.shots); i++)
	{
		player.shots[i].y -= 4;
        // reached top of screen
		if (player.shots[i].y <= 40)
		{
            // add explosion
            const struct explosion_t explosion = {
                .x = player.shots[i].x - 3,
                .y = 40,
                .clip = { 36, 24,  8,  8 },
                .timing = 0,
                .timeout = 16 * 24
            };
            arrput(explosions, explosion);
            arrdel(player.shots, i);
        }
	}
}

void update_player()
{
	const uint8_t* keys = SDL_GetKeyboardState(NULL);

	switch (player.state)
	{
	case PLAYER_ALIVE:
		// movement
		if (keys[SDL_SCANCODE_LEFT])
			player.x -= 1;
		if (keys[SDL_SCANCODE_RIGHT])
			player.x += 1;
		
		// check bounds	
		if (player.x < 14)
			player.x = 14;
		else if (player.x > WORLD_WIDTH - 31)
			player.x = WORLD_WIDTH - 31;

		// shooting
		if (player.shot_timing < PLAYER_SHOT_TIMEOUT)
            player.shot_timing += app.frame_time;
		else if (keys[SDL_SCANCODE_SPACE])
		{
			// shoot
			const SDL_Point shot = { player.x + 8, PLAYER_Y };
			arrput(player.shots, shot);
			player.shot_timing = 0;
		}

		break;
	case PLAYER_DEAD:
		player.death_timing += app.frame_time;
		if (player.death_timing >= PLAYER_DEATH_TIMEOUT)
		{
			player.state = PLAYER_ALIVE;
			player.x = 14;
			player.lives--;
			player.death_timing = 0;
		}

		break;
	}
}

void render_player_shots()
{
    for (int i = 0; i < arrlen(player.shots); i++)
	{
		SDL_Rect rect = {
			APP_SCALE * player.shots[i].x,
			APP_SCALE * player.shots[i].y,
			APP_SCALE,
			APP_SCALE * 4,
		};
		SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(app.renderer, &rect);
	}
}

void render_player()
{
    static const SDL_Rect clip = { 0, 8, 16, 8 };
    if (player.state == PLAYER_ALIVE)
	    app_render_clip(&clip, player.x, PLAYER_Y);
}

// interactions

void process_collisions()
{
    // horde shots colliding with
    for (int i = 0; i < arrlen(horde.shots); i++)
    {
        const int hshot_x = horde.shots[i].x;
        const int hshot_y = horde.shots[i].y;

        // player
        if (hshot_y >= PLAYER_Y && hshot_y <= PLAYER_Y + 6 &&
            hshot_x >= player.x + 2 && hshot_x <= player.x + 15)
        {
            // create explosion effect
            const struct explosion_t explosion = {
                .x = hshot_x - 1,
                .y = hshot_y,
                .clip = { 24, 40,  6,  8 },
                .timing = 0,
                .timeout = 16 * 24
            };
            arrput(explosions, explosion);
            arrdel(horde.shots, i);

            // player dead
            player.state = PLAYER_DEAD;
            player.x = 10000;
        }
        else for (int j = 0; j < arrlen(player.shots); j++) // player shots
        {
            const int pshot_x = player.shots[j].x;
            const int pshot_y = player.shots[j].y;

            if (pshot_y <= hshot_y && abs(pshot_x - hshot_x) <= 1)
            {
                const struct explosion_t explosion1 = {
                    .x = hshot_x - 1,
                    .y = hshot_y,
                    .clip = { 24, 40,  6,  8 },
                    .timing = 0,
                    .timeout = 16 * 24
                };
                arrput(explosions, explosion1);
                arrdel(horde.shots, i);

                const struct explosion_t explosion2 = {
                    .x = pshot_x - 3,
                    .y = pshot_y,
                    .clip = { 36, 24,  8,  8 },
                    .timing = 0,
                    .timeout = 16 * 24
                };
                arrput(explosions, explosion2);
                arrdel(player.shots, j);

                break;
            }
        }
    }

    // player shots colliding with
    for (int i = 0; i < arrlen(player.shots); i++)
    {
        const SDL_Rect prect = { player.shots[i].x, player.shots[i].y - 4, 1, 1 };

        // tourist
        const SDL_Rect tourist_rect = {tourist.x, TOURIST_Y, 24, 8};
        if (SDL_HasIntersection(&tourist_rect, &prect))
        {
            tourist.state = TOURIST_DYING;
            game_score += tourist.score_value;
            if (rand() % 2 == 0)
                player.lives++;
            
            arrdel(player.shots, i);
            continue;
        }

        // horde
        for (int j = 0; j < arrlen(horde.invaders); j++)
        {
            SDL_Rect irect = {
                horde.invaders[j].x + 2, horde.invaders[j].y, 8, 8
            };
            int score_inc = 30;
            // 16, 24, 32
            if (horde.invaders[j].clip.y == 24)
            {
                score_inc = 20;
                irect.x -= 1;
                irect.w += 3;
            }
            else if (horde.invaders[j].clip.y == 32)
            {
                score_inc = 10;
                irect.x -= 2;
                irect.w += 4;
            }

            if (SDL_HasIntersection(&prect, &irect))
            {
                // add explosion
                const struct explosion_t explosion = {
                    .x = horde.invaders[j].x,
                    .y = horde.invaders[j].y,
                    .clip = { 0, 40, 13,  8 },
                    .timing = 0,
                    .timeout = 16 * 16
                };
                arrput(explosions, explosion);
                arrdel(player.shots, i);

                if (j <= horde.invaders_updated)
                    horde.invaders_updated--;
                arrdel(horde.invaders, j);
                break;

                game_score += score_inc;
            }
        }
    }
}

void render_explosions()
{
    // explosions
    for (int i = 0; i < arrlen(explosions); i++)
        app_render_clip(&explosions[i].clip, explosions[i].x, explosions[i].y);
}

// base

void game_start()
{
    game_state = GAME_PLAYING;
    // score
    game_score = 0;
    game_hi_score = 1000; // load hi-score

    // explosions
    explosions = NULL;
    // horde
    horde.state = HORDE_STARTING;
    horde.invaders = NULL;
    horde.invaders_updated = 0;
    horde.shots = NULL;
    horde.shot_timing = 0;
    horde.shot_timeout = 16 * 32 * (rand() % 2 + 1);
    // tourist
    tourist.state = TOURIST_UNAVAILABLE;
    tourist.x = -1000.f;
    tourist.score_value = 100;
    tourist.spawn_timing = 0;
    tourist.spawn_timeout = 1024 * (rand() % (TOURIST_SPAWN_TIMEOUT_MAX - \
        TOURIST_SPAWN_TIMEOUT_MIN + 1) + TOURIST_SPAWN_TIMEOUT_MIN);
    tourist.death_timing = 0;
    tourist.score_timing = 0;
    // player
    player.x = 14;
	player.lives = 3;
    player.shots = NULL;
	player.death_timing = 0;
	player.shot_timing = 0;
}

void game_playing()
{
    // updating

    update_explosions();
    update_player();
    if (player.state == PLAYER_ALIVE)
        update_horde();
    update_tourist();

    update_horde_shots();
    update_player_shots();

    process_collisions();

    // rendering
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    // useless bar. Just to resemble the original game
    SDL_SetRenderDrawColor(app.renderer, 32, 255, 32, 255); // #20ff20
    static const SDL_Rect rect = {
        APP_SCALE * 0, APP_SCALE * 247, APP_SCALE * 224, APP_SCALE
    };
    SDL_RenderFillRect(app.renderer, &rect);  

    render_player_shots();
    render_horde_shots();

    render_tourist();
    render_horde();
    render_player();

    render_explosions();

    // ui
    render_scores();
    // live counter
    char player_lives[] = {'0' + player.lives};
    app_render_text(player_lives, 8, WORLD_HEIGHT - 16);
    // live cannons
    static const SDL_Rect cannon_clip = { 0, 8, 16, 8 };
    for (int i = 0; i < player.lives - 1; i++)
        app_render_clip(&cannon_clip, 24 + i * 16, WORLD_HEIGHT - 16);

    // useless arcade coin easteregg
    app_render_text("CREDIT -1", WORLD_WIDTH - 80, WORLD_HEIGHT - 16);
}

void game_process_event()
{

}

void game_frame()
{
    switch (game_state)
    {
    case GAME_PLAYING:
        game_playing();
        break;
    case GAME_PAUSED:
        break;
    case GAME_OVER:
        break;
    }
}

/* MENU STATE */

char menu[][10] = { "PLAY", "EXIT" };
int menu_selection;

void menu_state_init()
{
    menu_selection = 0;
}

void menu_process_event()
{
    if (app.event.type == SDL_KEYDOWN && !app.event.key.repeat)
    {
        switch (app.event.key.keysym.sym)
        {
        case SDLK_DOWN:
            menu_selection = (menu_selection + 1) % 2;
            break;
        case SDLK_UP:
            if (--menu_selection < 0)
                menu_selection = 1;
            break;
        case SDLK_RETURN:
            if (menu_selection == 1)
                app.state = APP_QUITTING;
            else
                app.state = APP_IN_GAME;
                game_start();
            break;
        }
    }
}

void menu_frame()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    app_render_text(menu[0], 0, 0);
    app_render_text(menu[1], 0, 16);
}

/* MAIN LOOP AND ENTRY POINT */

void app_main_loop()
{
    uint32_t start = 0, event_wait_timeout = 1000 / APP_FPS;

    while (app.state != APP_QUITTING)
    {
        // beginning of frame. Get current time.
        start = SDL_GetTicks();

        // wait for event
        if (SDL_WaitEventTimeout(&app.event, event_wait_timeout))
        {
            if (app.event.type == SDL_QUIT)
                app.state = APP_QUITTING;
            else switch (app.state)
            {
            case APP_IN_MENU:
                menu_process_event();
                break;
            case APP_IN_GAME:
                game_process_event();
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
            switch (app.state)
            {
            case APP_IN_MENU:
                menu_frame();
                break;
            case APP_IN_GAME:
                game_frame();
                break;
            }
            SDL_RenderPresent(app.renderer);

            app.frame_time = 0; // reset frame time
            event_wait_timeout = 1000 / APP_FPS; // reset event wait timeout
        }

        // accumulate frame time
        app.frame_time += SDL_GetTicks() - start;
    }
}

// entry

int main(int argc, char** args)
{
    // initialization

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);

    SDL_ShowCursor(SDL_DISABLE);

    app.state = APP_IN_MENU;
    menu_state_init();
    app.window = SDL_CreateWindow(
        "",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        APP_SCALE * WORLD_WIDTH,
        APP_SCALE * WORLD_HEIGHT,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS
    );
    app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);
    app.frame_time = 0;

    atlas = IMG_LoadTexture(app.renderer, APP_RESOURCE_DIR "/atlas.png");
    SDL_assert(atlas);

    TTF_Init();
    {
        TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "/font.ttf", APP_FONT_PTSIZE);
        SDL_assert(font);

        SDL_Surface* font_surf = TTF_RenderUTF8_Solid(
            font, APP_CHARACTERS_MONO, (SDL_Color){ 255, 255, 255, 255 }
        );

        // create the texture and free the surface
        font_atlas = SDL_CreateTextureFromSurface(app.renderer, font_surf);
        SDL_FreeSurface(font_surf);

        TTF_CloseFont(font);
    }
    TTF_Quit();
    
    app_main_loop(); // execution

    // termination

    SDL_DestroyTexture(font_atlas);
    SDL_DestroyTexture(atlas);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);

    IMG_Quit();
    SDL_Quit();
}
