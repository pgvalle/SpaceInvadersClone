#include "fsm.h"
#include "app.h"

#include <stdlib.h>
#include <stdbool.h>

// First state
void start_state_init();
void start_state_destroy();
void start_state_update();
void start_state_render();

// stack
fsm_state_t* stack = NULL;
int top = -1;
// stack update status
bool pushing = false, poping = false;

bool initialized = false;

void fsm_init()
{
    stack = NULL;
    top = 0;
    // changes are going to be applied immediately
    pushing = poping = false;

    // push initial state to stack
    const fsm_state_t first = {
        .init    = start_state_init,
        .destroy = start_state_destroy,
        .update  = start_state_update,
        .render  = start_state_render
    };
    arrput(stack, first);

    stack[top].init(); // Init the state

    initialized = true;
}

void fsm_destroy()
{
    if (!initialized)
        return;
    
    // destroy all states
    for (; top >= 0; top--)
        stack[top].destroy();

    // free stack
    arrfree(stack);
}

bool fsm_empty()
{
    return arrlen(stack) > 0;
}

void fsm_update()
{
    if (!initialized || fsm_empty())
        return;
    
    if (pushing && poping) // replacing
    {
        stack[top].destroy();
        arrdel(stack, top);
        stack[top].init();
    }
    else if (pushing)
        stack[++top].init();
    else if (poping) // stack may get empty !!!
    {
        stack[top].destroy();
        arrdel(stack, top--);
    }

    // already did what had to be done
    pushing = poping = false;
}

void fsm_update_current()
{
    stack[top].update();
}

void fsm_render_current()
{
    stack[top].render();
    SDL_RenderPresent(app.renderer);
}

void fsm_push(fsm_state_t state)
{
    if (!fsm_empty() && !pushing && !poping)
    {
        arrput(stack, state);
        pushing = true;
    }
}

void fsm_pop() {
    if (!fsm_empty() && !pushing && !poping)
        poping = true;
}

void fsm_replace(fsm_state_t state)
{
    if (!fsm_empty() && !pushing && !poping)
    {
        arrput(stack, state);
        pushing = poping = true;
    }
}
