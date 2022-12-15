#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdbool.h>

struct GameData;

void cannon_initialize(struct GameData *game, int y);
void cannon_destroy(struct GameData *game);
void cannon_update(struct GameData *game);
void cannon_render(struct GameData *game);

// 5 rows with 11 invaders each
#define INVADERS_COUNT 55

// invader types
#define INVADER0 0   // special one
#define INVADER1 1
#define INVADER2 2
#define INVADER3 3

void invaders_initialize(struct GameData *game, int x, int y);
void invaders_destroy(struct GameData *game);
void invaders_processEvents(struct GameData *game);
void invaders_update(struct GameData *game);
void invaders_render(struct GameData *game);

#define GAME_SCALING_FACTOR 3

// ~~ 60fps
#define EVENT_TIMEOUT_MS 17

struct GameData
{
	bool quit;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;
	int64_t frametime;

	// texture data
	SDL_Texture *inv0,
				*inv1,
				*inv2,
				*inv3,
				*death123;

	struct CannonData
	{
		SDL_Texture *tex;
		int x,
			y;
		int lives;
		int64_t shot_cooldown;
		int64_t death_anim_timeout;
		int anim_frame;
	} cannon;

	struct InvadersData
	{
		struct InvaderInstance
		{
			int type;
			int x,
				y;
			bool has_been_updated;
			bool alive;
			int death_anim_timeout;
			bool has_played_death_anim;
			int anim_frame;
		} instances[INVADERS_COUNT + 1]; // the "+1" is the special one
		
		int anim_timeout;

		bool sideways_right;
		int sideways_moves_count;
	} invaders;
};
