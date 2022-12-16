#include <stdlib.h>
#include <stdio.h>

#include "gamedata.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void gameloop(struct GameData *game)
{
	Uint32 before = 0;

	while (!game->quit)
	{
		// compute frametime
		game->frametime = SDL_GetTicks() - before;
		before = SDL_GetTicks();

		// process events
		while (SDL_PollEvent(&game->event))
		{
			if (game->event.type == SDL_QUIT)
			{
				game->quit = true;
				return;
			}
			invaders_processEvents(game);
		}

		// update game entities
		cannon_update(game);
		invaders_update(game);		

		// rendering
		SDL_RenderClear(game->ren);
		cannon_render(game);
		invaders_render(game);
		SDL_RenderPresent(game->ren);
	}
}

int main(int argc, char const** args)
{
	// SDL info
	{
		SDL_version version;
		// compiled
		SDL_VERSION(&version);
		printf("Compiled against SDL %d.%d.%d\n", version.major,
			version.minor, version.patch);
		// linked
		SDL_GetVersion(&version);
		printf("Linked against SDL %d.%d.%d\n", version.major,
			version.minor, version.patch);
	}

	// initializing
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	struct GameData *data = malloc(sizeof(*data));
	data->quit = false;
	data->win = SDL_CreateWindow("Space Invaders Clone",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCALING_FACTOR * CANVAS_WIDTH, SCALING_FACTOR * CANVAS_HEIGHT, 0);
	data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED);
	cannon_initialize(data);
	invaders_initialize(data);

	// execution
	gameloop(data);

	// Terminating
	invaders_destroy(data);
	cannon_destroy(data);
	SDL_DestroyRenderer(data->ren);
	SDL_DestroyWindow(data->win);
	free(data);

	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
