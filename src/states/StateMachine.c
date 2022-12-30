#include "StateMachine.h"
#include "StatesInternal.h"

#include <stdlib.h>
#include <stdbool.h>

// current state
Function DestroyState, UpdateState, RenderState;
Function InitNewState, DestroyNewState, UpdateNewState, RenderNewState;

bool changeScheduled;

void InitStateMachine()
{
    InitGameplayState();

    DestroyState = DestroyGameplayState;
    UpdateState = UpdateGameplayState;
    RenderState = RenderGameplayState;

    InitNewState = NULL;
    DestroyNewState = NULL;
    UpdateNewState = NULL;
    RenderNewState = NULL;
    
    changeScheduled = false;
}

void DestroyStateMachine()
{
    // destroy all states
}

void ScheduleStateChange(
    Function Init,
    Function Destroy,
    Function Update,
    Function Render
)
{
    InitNewState = Init;
    DestroyNewState = Destroy;
    UpdateNewState = Update;
    RenderNewState = Render;

    changeScheduled = true;
}

void UpdateStateChanges()
{
    if (changeScheduled)
    {
        DestroyState();
        InitNewState();

        DestroyState = DestroyNewState;
        UpdateState = UpdateNewState;
        RenderState = RenderNewState;

        InitNewState = NULL;
        DestroyNewState = NULL;
        UpdateNewState = NULL;
        RenderNewState = NULL;

        changeScheduled = false;
    }
}

void UpdateCurrentState()
{
    UpdateState();
}

void RenderCurrentState()
{
    RenderState();
}