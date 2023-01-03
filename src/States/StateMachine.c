#include "StateMachine.h"
#include "StatesInternal.h"
#include "../Application.h"
#include "../Utils/stb_ds.h"

#include <stdlib.h>
#include <stdbool.h>

// current state
State* states;
int current;

bool pushing, poping;
bool changeScheduled;

void InitStateMachine()
{
    // push initial state to stack
    const State initialState = {
        .Init = InitGameplayState,
        .Destroy = DestroyGameplayState,
        .Update = UpdateGameplayState,
        .Render = RenderGameplayState
    };
    arrput(states, initialState);
    current = 0; // current state index is 0

    states[current].Init(); // Init the state

    // already pushed, not poping and changes already applied
    pushing = false;
    poping = false;
    changeScheduled = false;
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
    if (changeScheduled)
        return;

    arrput(states, state);
    pushing = true;
    changeScheduled = true;
}

void PopState()
{
    if (changeScheduled)
        return;

    poping = true;
    changeScheduled = true;
}

void ReplaceState(State state)
{
    if (changeScheduled)
        return;

    arrput(states, state);
    pushing = poping = true;
    changeScheduled = true;
}

void UpdateStateMachine()
{
    if (!changeScheduled)
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
    changeScheduled = false;
}

void UpdateCurrentState()
{
    states[current].Update();
}

void RenderCurrentState()
{
    states[current].Render();
}