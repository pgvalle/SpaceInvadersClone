#ifndef STATES_INTERNAL_H
#define STATES_INTERNAL_H

#include "../Utils/Function.h"

// ========================================================================= //
// State Machine
// ========================================================================= //

// This function is called by the states. Changes aren't applied just yet.
void ScheduleStateChange(
    Function Init,
    Function Destroy,
    Function Update,
    Function Render
);

// ========================================================================= //
// State Methods
// ========================================================================= //

void InitGameplayState();
void DestroyGameplayState();
void UpdateGameplayState();
void RenderGameplayState();


#endif // STATES_INTERNAL_H