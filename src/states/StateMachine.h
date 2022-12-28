#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../util/Function.h"


void InitStateMachine();
// This function is called by the states. Changes aren't applied just yet.
void ScheduleStateChange(Function init, Function destroy, Function update,
    Function render);
// The state changes are only applied when this function is called.
void UpdateStateChanges();

void UpdateCurrentState();
void RenderCurrentState();


#endif // STATE_MACHINE_H