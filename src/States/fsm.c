#include "fsm.h"
#include "StatesInternal.h"
#include "../Application.h"
#include "../Utils/Render.h"
#include "../Utils/stb_ds.h"

#include <stdlib.h>
#include <stdbool.h>

// stack
state_t* stack;
int top;

bool pushing, poping;

void fsm_init()
{
    // push initial state to stack
    const state_t initialState = {
        .init = InitMenuState,
        .destroy = DestroyMenuState,
        .update = UpdateMenuState,
        .render = RenderMenuState
    };
    arrput(stack, initialState);
    top = 0; // top index is 0

    stack[top].init(); // Init the state

    // changes already applied
    pushing = false;
    poping = false;
}

void fsm_destroy()
{
    // destroy all states
    for (; top >= 0; top--)
        stack[top].destroy();

    // free stack
    arrfree(stack);
}

void fsm_push(state_t state)
{
    if (!pushing && !poping)
    {
        arrput(stack, state);
        pushing = true;
    }
}

void fsm_pop()
{
    if (!pushing && !poping)
        poping = true;
}

void fsm_replace(state_t state)
{
    if (!pushing && !poping)
    {
        arrput(stack, state);
        pushing = poping = true;
    }
}

void fsm_update()
{
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
        arrdel(stack, top);

        if (--top == -1)
            app.shouldClose = true;
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
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
    stack[top].render();
    SDL_RenderPresent(app.renderer);
}