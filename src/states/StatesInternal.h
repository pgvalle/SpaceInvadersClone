#ifndef STATES_INTERNAL_H
#define STATES_INTERNAL_H

#include "../util/Function.h"

// ========================================================================= //
// State Machine
// ========================================================================= //

// This function is called by the states. Changes aren't applied just yet.
void ScheduleStateChange(
    Function init,
    Function destroy,
    Function update,
    Function render
);

// ========================================================================= //
// State Methods
// ========================================================================= //

void InitGameplayState();
void DestroyGameplayState();
void UpdateGameplayState();
void RenderGameplayState();


#endif // STATES_INTERNAL_H