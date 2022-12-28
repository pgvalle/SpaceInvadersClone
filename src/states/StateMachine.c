#include "StateMachine.h"
#include "States.h"

#include <stdlib.h>
#include <stdbool.h>

Function currentDestroy, currentUpdate, currentRender;
Function newInit, newDestroy, newUpdate, newRender;

bool changeScheduled;

void InitStateMachine()
{
    InitMenuState();

    currentDestroy = DestroyMenuState;
    currentUpdate = UpdateMenuState;
    currentRender = RenderMenuState;

    newInit = NULL;
    newDestroy = NULL;
    newUpdate = NULL;
    newRender = NULL;
    
    changeScheduled = false;
}

void ScheduleStateChange(Function init, Function destroy, Function update,
    Function render)
{
    newInit = init;
    newDestroy = destroy;
    newUpdate = update;
    newRender = render;

    changeScheduled = true;
}

void UpdateStateChanges()
{
    if (changeScheduled)
    {
        currentDestroy();
        newInit();

        currentDestroy = newDestroy;
        currentUpdate = newUpdate;
        currentRender = newRender;

        newInit = NULL;
        newDestroy = NULL;
        newUpdate = NULL;
        newRender = NULL;
    }
}

void UpdateCurrentState()
{
    currentUpdate();
}

void RenderCurrentState()
{
    currentRender();
}