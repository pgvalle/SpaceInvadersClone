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

// These functions are called by the states. Changes aren't applied just yet.
// Only the first call will be considered.
// Ex:
// PushState(state);
// ...
// PopState();
// In This example, PopState is ignored because PushState was called first.
void PushState(State state);
void PopState();
void ReplaceState(State state);

// ========================================================================= //
// State Methods
// ========================================================================= //

void InitGameplayState();
void DestroyGameplayState();
void UpdateGameplayState();
void RenderGameplayState();


#endif // STATES_INTERNAL_H