#ifndef STATES_INTERNAL_H
#define STATES_INTERNAL_H

#include "../Utils/Function.h"

// ========================================================================= //
// State Machine
// ========================================================================= //

struct State
{
    Function Init, Destroy, Update, Render;
};
typedef struct State State;

// These function are called by the states. Changes aren't applied just yet.
// Only the first call will be considered.
// Ex:
// PushState(state);
// ...
// PopState();
// In This example, PopState and any other after PushState are be ignored.
void PushState(State state), PopState(), ReplaceState(State state);

// ========================================================================= //
// State Methods
// ========================================================================= //

void InitGameplayState();
void DestroyGameplayState();
void UpdateGameplayState();
void RenderGameplayState();


#endif // STATES_INTERNAL_H