#ifndef STATES_INTERNAL_H
#define STATES_INTERNAL_H

#include "../Utils/Function.h"

// ========================================================================= //
// State Machine
// ========================================================================= //

typedef struct fsm_state_t {
    Function init, destroy, update, render;
} fsm_state_t;

// These functions are called by the states. Changes aren't applied just yet.
// Only the first call will be considered.
// Ex:
// fsm_push(state);
// ...
// fsm_pop();
// In This example, fsm_pop is ignored because fsm_push was called first.
void fsm_push(fsm_state_t state);
void fsm_pop();
void fsm_replace(fsm_state_t state);

// ========================================================================= //
// State Methods
// ========================================================================= //

void InitGameplayState();
void DestroyGameplayState();
void UpdateGameplayState();
void RenderGameplayState();

void InitMenuState();
void DestroyMenuState();
void UpdateMenuState();
void RenderMenuState();


#endif // STATES_INTERNAL_H