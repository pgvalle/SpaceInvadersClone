#include <stdlib.h>
#include <stdio.h>

#include "gamedata.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>



bool waitRemainingEventTimeout(GameData *data)
{
	static int64_t before, now;
	bool occurred = SDL_WaitEventTimeout(&data->event, data->event_timeout);
	before = now;
	now = SDL_GetTicks();
	if (occurred)
	{
		data->event_timeout -= now - before;
		if (data->event_timeout <= 0)
			data->event_timeout = 0;
	}
	else
	{
		data->event_timeout = EVENT_TIMEOUT_MS;
		data->event.type = 0; // clear event type
	}
		

	return occurred;
}

void gameloop(GameData *data)
{
	while (!data->quit)
	{
		if (waitRemainingEventTimeout(data))
		{
			// process input events
			if (data->event.type == SDL_QUIT)
			{
				data->quit = true;
				return;
			}
		}
		else
		{
			// process time events
			cannon_processEvents(data);
			invaders_update(data);
		}

		

		SDL_RenderClear(data->ren);
		// cannon_render(data);
		invaders_render(data);
		SDL_RenderPresent(data->ren);
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

	// printf("arguments: ");
	// for (int i = 1; i < argc; i++)
	// 	printf("\"%s\" ", args[i]);
	// printf("\b\n");

	// initializing
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	GameData *data = malloc(sizeof(*data));
	data->quit = false;
	data->win = SDL_CreateWindow("Space Invaders Clone",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		GAME_SCALING_FACTOR*224, GAME_SCALING_FACTOR*256, 0);
	data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED);
	data->event_timeout = EVENT_TIMEOUT_MS;
	cannon_initialize(data);
	invaders_initialize(data);

	// execution
	gameloop(data);

	// Terminating
	invaders_destroy(data);
	cannon_destroy(data);
	SDL_DestroyRenderer(data->ren);
	SDL_DestroyWindow(data->win);

	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
