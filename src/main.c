#include "core/app.h"

int main(int argc, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    TTF_Init();

    app_init();
    app_run();
    app_destroy();

    SDL_Quit();
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();

    return 0;
}
