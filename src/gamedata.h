#include <SDL2/SDL.h>

#include <stdint.h>
#include <stdbool.h>

#define SCALING_FACTOR 3
#define CANVAS_WIDTH  224
#define CANVAS_HEIGHT 256

// 5 rows with 11 invaders each
#define HORDE_SIZE 55

struct GameData
{
	bool quit;
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;
	int64_t frametime;

	// textures
	SDL_Texture *inv0,
				*inv1,
				*inv2,
				*inv3,
				*death123,
				*cannonTex;

	struct CannonData
	{
		int x,
			y;
		int lives;
		int shot_cooldown;
		int death_anim_timeout;
		int anim_frame;
	} cannon;

	struct InvadersData
	{
		struct Invader123Instance
		{
			int type;
			int x,
				y;
			bool alive;
			bool move_anim_done;
			int move_anim_frame;
			int death_anim_time;
		} horde[HORDE_SIZE];

		bool horde_locked;

		struct InvaderSpecial
		{
			int x,
				y;
			bool alive;
		} special;
		
		int move_anim_time;
		int move_anim_timeout;

		bool sideways_right;
		int sideways_moves_count;
	} invaders;
};


// ==============================================
// INVADERS
// ==============================================

// types (not counting the special one)
#define INVADER1 1
#define INVADER2 2
#define INVADER3 3

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 60

#define HORDE_MOVE_ANIM_TIMEOUT_INIT 20
#define INVADER_DEATH_ANIM_TIMEOUT 300

void invaders_initialize(struct GameData *game);
void invaders_destroy(struct GameData *game);
void invaders_processEvents(struct GameData *game);
void invaders_update(struct GameData *game);
void invaders_render(struct GameData *game);


// ==============================================
// CANNON
// ==============================================

#define CANNON_X_INIT 20
#define CANNON_Y      230

#define CANNON_SHOT_COOLDOWN 1000

void cannon_initialize(struct GameData *game);
void cannon_destroy(struct GameData *game);
void cannon_update(struct GameData *game);
void cannon_render(struct GameData *game);