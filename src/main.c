#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, char const** args)
{
	// basic commandline info
	printf("arguments: ");
	for (int i = 1; i < argc; i++)
		printf("\"%s\" ", args[i]);
	printf("\b\n");

	// SDL version info
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

	

	// Terminating
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
