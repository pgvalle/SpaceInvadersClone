#include "../App.h"

#define GET_APP \
    struct App* app = GetAppInstance()

void UpdateGameplayState()
{
    Horde_Update();
    Cannon_Update();
}

void RenderGameplayState()
{
    GET_APP;

    SDL_RenderClear(app->renderer);
    Horde_Render();
    Cannon_Render();
    SDL_RenderPresent(app->renderer);
}