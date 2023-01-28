#include "shots.h"
#include "core.h"
#include "game/internal.h"
#include "utils/stb_ds.h"

struct horde_shot_t* horde_shots = NULL;
struct player_shot_t* player_shots = NULL;

void shots_init()
{
	arrfree(horde_shots);
	arrfree(player_shots);
}

void shots_update()
{
	for (int i = 0; i < arrlen(horde_shots); i++)
	{
		// update position
		horde_shots[i].y += horde_shots[i].yvel;
		animation_update(&horde_shots[i].anim);

		if (horde_shots[i].y >= 240)
			arrdel(horde_shots, i);
	}

	for (int i = 0; i < arrlen(player_shots); i++)
	{
		// same
		player_shots[i].y -= 4;

		if (player_shots[i].y <= 40)
			arrdel(player_shots, i);
		else
		{
			const int player_shot_x = player_shots[i].x;
			const int player_shot_y = player_shots[i].y;
			for (int j = 0; j < arrlen(horde_shots); j++)
			{
				const int horde_shot_x = horde_shots[j].x;
				const int horde_shot_y = horde_shots[j].y;

				if (player_shot_y <= horde_shot_y &&
					player_shot_x >= horde_shot_x && player_shot_x < horde_shot_x + 3)
				{
					arrdel(horde_shots, j);
					arrdel(player_shots, i);
					break;
				}
			}
		}
	}
}

void shots_render()
{
	for (int i = 0; i < arrlen(horde_shots); i++)
	{
		animation_render(&horde_shots[i].anim, horde_shots[i].x, horde_shots[i].y);
	}

	for (int i = 0; i < arrlen(player_shots); i++)
	{
		// same
		SDL_Rect rect = {
			APP_SCALE * player_shots[i].x,
			APP_SCALE * player_shots[i].y,
			APP_SCALE,
			APP_SCALE * 4,
		};
		SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
		SDL_RenderDrawRect(app.renderer, &rect);
	}
}

