/**
 * This header is, as the name suggests, may only be accessed from core
 */
#ifndef CORE_INTERNAL_H
#define CORE_INTERNAL_H

#include <stdbool.h>

/**
 * ASSET MANAGER
 */

void asset_man_init();
void asset_man_destroy();

/**
 * STATE MACHINE
 */

void fsm_init();
void fsm_destroy();

bool fsm_empty();

// State changes are only applied when this function is called.
// May be called only after fsm_update_current and fsm_render_current were called.
void fsm_update();

void fsm_process_event_for_current();
void fsm_update_current();
void fsm_render_current();


#endif // CORE_INTERNAL_H
