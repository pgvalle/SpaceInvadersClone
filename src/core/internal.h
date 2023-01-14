/**
 * This header is, as the name suggests, may only be accessed from core
 */
#ifndef CORE_INTERNAL_H
#define CORE_INTERNAL_H

#include <stdbool.h>

/**
 * STATE MACHINE
 */

void fsm_init();
void fsm_destroy();

bool fsm_empty();

// State changes are only applied when this function is called.
// May be called only after fsm_update_current and fsm_render_current were called.
void fsm_update();

void fsm_update_current_state();
void fsm_render_current_state();

/**
 * ASSET MANAGER
 */

void asset_man_init();
void asset_man_destroy();


#endif // CORE_INTERNAL_H
