#include "player.h"
#include "core.h"
#include "game/internal.h"

struct player_t player;

void player_init()
{
	player.x = 16;
	player.death_timer = (t1mer_t) {
		.has_timed_out = false,
		.time = 0,
		.timeout = PLAYER_DEATH_TIMEOUT
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
			player.x -= 2;
		if (keys[SDL_SCANCODE_RIGHT])
			player.x += 2;
		
		// check bounds	
		if (player.x < 14)
			player.x = 14;
		else if (player.x > WORLD_WIDTH - 31)
			player.x = WORLD_WIDTH - 31;
			
		// shooting
				
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
