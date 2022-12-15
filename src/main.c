#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define TEX_INVADER0 0
#define TEX_INVADER1 1
#define TEX_INVADER2 2
#define TEX_INVADER3 3

typedef struct game_data_t
{
	bool quit;
	
	SDL_Window *win;
	SDL_Renderer *ren;
	SDL_Event event;

	SDL_Texture *tex[4];
} game_data_t;

struct invader_instance_t
{
	int class;
	int frame_i;

	int x, y;

	bool alive;
};

void gameloop(game_data_t *data)
{
	while (!data->quit)
	{
		if (!SDL_PollEvent(&data->event))
			continue;

		if (data->event.type == SDL_QUIT){
			data->quit = true;
			printf("Quitting!\n");
		}

		SDL_RenderClear(data->ren);
		
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

	// commandline info
	printf("arguments: ");
	for (int i = 1; i < argc; i++)
		printf("\"%s\" ", args[i]);
	printf("\b\n");

	// initializing
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	game_data_t *data = malloc(sizeof(*data));
	data->quit = false;
	data->win = SDL_CreateWindow("Space Invaders",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		448, 512, 0);
	data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED);
	data->tex[TEX_INVADER0] = IMG_LoadTexture(data->ren, "./res/invader0.png");
	SDL_assert(data->tex != NULL);

	// execution
	gameloop(data);

	// Terminating
	SDL_DestroyTexture(data->tex[TEX_INVADER0]);
	SDL_DestroyRenderer(data->ren);
	SDL_DestroyWindow(data->win);

	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
