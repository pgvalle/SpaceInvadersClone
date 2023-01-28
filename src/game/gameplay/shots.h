#ifndef SHOTS_H
#define SHOTS_H

#include "utils/animation.h"

extern struct horde_shot_t
{
	int x, y; // x is fixed.
	int yvel;
	animation_t anim;
}* horde_shots;

extern struct player_shot_t
{
	int x, y; // x is fixed here too
}* player_shots;

void shots_init();
void shots_update();
void shots_render();

#endif // SHOTS_H

