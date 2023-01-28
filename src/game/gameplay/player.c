#include "player.h"
#include "core.h"
#include "utils/stb_ds.h"
#include "game/internal.h"
#include "shots.h"

struct player_t player;

void player_init()
{
	player.x = 14;
	player.deaths = 0;
	player.death_timer = (t1mer_t) {
		.has_timed_out = false,
		.time = 0,
		.timeout = PLAYER_DEATH_TIMEOUT
	};
	player.shot_timer = (t1mer_t) {
		.has_timed_out = false,
		.time = 0,
		.timeout = 16 * (1000 / 16)
	};
}

void player_update()
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
		if (!player.shot_timer.has_timed_out)
			timer_update(&player.shot_timer);
		else if (keys[SDL_SCANCODE_SPACE])
		{
			// shoot
			const struct player_shot_t shot = { player.x + 8, PLAYER_Y };
			arrput(player_shots, shot);
			timer_reset(&player.shot_timer);
		}

		break;
	case PLAYER_DEAD:
		timer_update(&player.death_timer);
		if (player.death_timer.has_timed_out)
		{
			player.state = PLAYER_ALIVE;
			player.x = 14;
			player.deaths++;
			timer_reset(&player.death_timer);
		}

		break;
	}
}

void player_render()
{
	if (player.state == PLAYER_DEAD)
		return;

	static const SDL_Rect clip = { 0, 8, 16, 8 };
	const SDL_Rect scale = {
		APP_SCALE * player.x,
		APP_SCALE * PLAYER_Y,
		APP_SCALE * clip.w,
		APP_SCALE * clip.h
	};

	SDL_RenderCopy(app.renderer, atlas, &clip, &scale);
}
