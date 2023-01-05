#include "../fsm.h"
#include "../StatesInternal.h"
#include "../../Application.h"

void InitMenuState()
{

}

void DestroyMenuState()
{

}

void UpdateMenuState()
{
    if (app.event.type == SDL_KEYDOWN && !app.event.key.repeat)
    {
        if (app.event.key.keysym.sym == SDLK_RETURN)
        {
            fsm_push((state_t) {
                InitGameplayState,
                    DestroyGameplayState,
                    UpdateGameplayState,
                    RenderGameplayState,
            });

            SDL_Log("Going to gameplay state...");
        }
        else if (app.event.key.keysym.sym == SDLK_q)
            app.shouldClose = true;
    }
}

void RenderMenuState()
{

}