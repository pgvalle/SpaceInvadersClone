#include "StatesInternal.h"
#include "../Application.h"
#include "../entities/Entities.h"

struct Horde horde;


void InitGameplayState()
{
    InitHorde(&horde);
}

void DestroyGameplayState()
{
    
}

void UpdateGameplayState()
{
    UpdateHorde(&horde);
}

void RenderGameplayState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    RenderHorde(&horde);

    SDL_RenderPresent(app.renderer);
}