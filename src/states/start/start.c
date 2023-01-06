#include "../../core/app.h"
#include "../../core/fsm.h"

void start_state_init()
{

}

void start_state_destroy()
{

}

void start_state_update()
{
    
}

void start_state_render()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_Rect rect = {0, 0, 100, 100};
    SDL_RenderFillRect(app.renderer, &rect);
    SDL_RenderPresent(app.renderer);
}