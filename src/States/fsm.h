#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

void fsm_init();
void fsm_destroy();

// State changes are only applied when this function is called.
// May be called only after fsm_update_current and fsm_render_current were called.
void fsm_update();

void fsm_update_current();
void fsm_render_current();


#endif // STATE_MACHINE_H