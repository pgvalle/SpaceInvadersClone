#ifndef CORE_FSM_H
#define CORE_FSM_H

#include "../utils/function.h"

// All states can be represented in 4 procedures:
// 1. initialize
// 2. update - game loop
// 3. render - game loop
// 4. destroy
typedef struct fsm_state_t
{
    function_t init, destroy, update, render;
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
// You may as well call fsm_push and fsm_pop (the order doesn't matter)
void fsm_replace(fsm_state_t state);


#endif // CORE_FSM_H
