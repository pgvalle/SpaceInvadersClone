#include "StateMachine.h"
#include "StatesInternal.h"
#include "../Application.h"
#include "../Utils/Render.h"
#include "../Utils/stb_ds.h"

#include <stdlib.h>
#include <stdbool.h>

// current state
State* states;
int current;

bool pushing, poping;

void InitStateMachine()
{
    // push initial state to stack
    const State initialState = {
        .Init = InitMenuState,
        .Destroy = DestroyMenuState,
        .Update = UpdateMenuState,
        .Render = RenderMenuState
    };
    arrput(states, initialState);
    current = 0; // current state index is 0

    states[current].Init(); // Init the state

    // already pushed, not poping and changes already applied
    pushing = false;
    poping = false;
}

void DestroyStateMachine()
{
    // destroy all states
    for (int i = 0; i < arrlen(states); i++)
        states[i].Destroy();

    // free stack
    arrfree(states);
}

void PushState(State state)
{
    if (pushing || poping)
        return;

    arrput(states, state);
    pushing = true;
}

void PopState()
{
    if (pushing || poping)
        return;

    poping = true;
}

void ReplaceState(State state)
{
    if (pushing || poping)
        return;

    arrput(states, state);
    pushing = poping = true;
}

void UpdateStateMachine()
{
    if (!pushing && !poping)
        return;

    if (pushing && poping)
    {
        states[current].Destroy();
        arrdel(states, current);
        states[current].Init();
    }
    else if (pushing)
        states[++current].Init();
    else if (poping) // stack may get empty !!!
    {
        states[current].Destroy();
        arrdel(states, current);

        if (--current == -1)
            app.shouldClose = true;
    }

    // nothing scheduled now
    pushing = poping = false;
}

void UpdateCurrentState()
{
    states[current].Update();
}

void RenderCurrentState()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);
    states[current].Render();
    SDL_RenderPresent(app.renderer);
}