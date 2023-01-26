#include "globals.h"
#include "gameplay/horde.h"
#include "gameplay/tourist.h"
#include "gameplay/player.h"
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
}

void gameplay_state_render()
{
    SDL_RenderClear(app.renderer);

    horde_render();
    tourist_render();
	player_render();
}
