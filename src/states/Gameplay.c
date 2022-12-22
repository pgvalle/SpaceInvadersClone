#include "../App.h"

#define GET_APP \
    struct App* app = GetAppInstance()

void UpdateGameplayState()
{
    // tought update order
    /* individual entities */
    // horde shots
    // player shots
    /* inter-entity */
    // check shot collisions with cannon, tourist and horde
    /* individual entities */
    // horde
    // player
    // tourist
    // Ui elements

    UpdateCannon();
    UpdateTourist();
}

void RenderGameplayState()
{
    GET_APP;

    SDL_RenderClear(app->renderer);
    RenderTourist();
    RenderCannon();
    SDL_RenderPresent(app->renderer);
}