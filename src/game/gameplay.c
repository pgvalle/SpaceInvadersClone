#include "gameplay/horde.h"
#include "gameplay/tourist.h"
#include "gameplay/player.h"
#include "gameplay/shots.h"
#include "core.h"

enum {
    GAMEPLAY_PLAYING,
    GAMEPLAY_PAUSED,
    GAMEPLAY_OVER
} gameplay_state;

void gameplay_state_init()
{
    horde_init();
    tourist_init();
	player_init();
    shots_init();
}

void gameplay_state_destroy()
{
    
}

void gameplay_state_process_event()
{
    if (app.event.type == SDL_QUIT)
        app.should_close = true;
}

void gameplay_state_update()
{
    horde_update();
    tourist_update();
	player_update();
    shots_update();
}

void gameplay_state_render()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    shots_render();
    horde_render();
    tourist_render();
	player_render();
}
