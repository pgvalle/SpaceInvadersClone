#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

void InitStateMachine();
void DestroyStateMachine();

// State changes are only applied when this function is called.
// May be called only after UpdateCurrentSTate and RenderCurrentState were called.
void UpdateStateChanges();

void UpdateCurrentState();
void RenderCurrentState();


#endif // STATE_MACHINE_H