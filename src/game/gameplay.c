#include "constants.h"
#include "gameplay/horde.h"
#include "gameplay/tourist.h"
#include "core.h"

void gameplay_state_init()
{
    horde_init();
    tourist_init();
}

void gameplay_state_destroy()
{
    
}

void gameplay_state_update()
{
    if (app.event.type == SDL_QUIT)
        app.should_close = true;

    horde_update();
    tourist_update();
}

void gameplay_state_render()
{
    horde_render();
    tourist_render();
}
