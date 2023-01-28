#ifndef PLAYER_H
#define PLAYER_H

#include "game/globals.h"
#include "utils/timer.h"

#define PLAYER_Y 224
#define PLAYER_DEATH_TIMEOUT (16 * 125) /* 2 seconds */

extern struct player_t
{
	enum {
		PLAYER_ALIVE = 0,
		PLAYER_DEAD
	} state;

	int x;
	int deaths;
	t1mer_t death_timer;
	t1mer_t shot_timer;
} player;

void player_init();
void player_update();
void player_render();


#endif // PLAYER_H

