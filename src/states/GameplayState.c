#include "StatesInternal.h"
#include "../Application.h"
#include "../entities/Entities.h"

struct Horde horde;
struct Cannon cannon;

void InitGameplayState()
{
    InitHorde(&horde);
    InitCannon(&cannon);
}

void DestroyGameplayState()
{
    
}

void UpdateGameplayState()
{
    UpdateHorde(&horde);
    UpdateCannon(&cannon);
}

void RenderGameplayState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    RenderHorde(&horde);
    RenderCannon(&cannon);

    SDL_RenderPresent(app.renderer);
}