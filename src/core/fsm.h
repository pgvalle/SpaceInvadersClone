#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

void fsm_init();
void fsm_destroy();

// State changes are only applied when this function is called.
// May be called only after fsm_update_current and fsm_render_current were called.
void fsm_update();

void fsm_update_current();
void fsm_render_current();

// All states can be represented in 4 procedures:
// 1. initialize
// 2. update - game loop
// 3. render - game loop
// 4. destroy
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


#endif // STATE_MACHINE_H